#include "samurai/airspy/channel.hpp"
#include "samurai/cbuffer.hpp"
#include <unistd.h>

namespace Samurai::Airspy {

Channel::Channel(void* fdn_ptr, Config cfg) {
    this->fdn = *(Channel::Foundation*)fdn_ptr;
    this->config = cfg;
}

Channel::~Channel() {
    StopStream();
}

Result Channel::Update(State s, bool force) {
    force = force || !configured;

    if (s.enableAGC != state.enableAGC || force) {
        int res = 0;
        res += airspy_set_lna_agc(fdn.device, s.enableAGC ? 1 : 0);
        res += airspy_set_mixer_agc(fdn.device, s.enableAGC ? 1 : 0);
        if (res != 0) {
            return Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        }
    }

    if (s.frequency != state.frequency || force) {
        if (airspy_set_freq(fdn.device, static_cast<uint32_t>(state.frequency)) != AIRSPY_SUCCESS) {
            return Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        }
    }

    if ((s.manualGain != state.manualGain && !s.enableAGC) || force) {

    }

    sleep(1);
    this->cb->Reset();

    this->configured = true;
    this->state = s;

    return Result::SUCCESS;
}

Result Channel::ReadStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (cb->Capacity() < size*2)
        return Result::ERROR;

    while(cb->Occupancy() < size*2) usleep(1000);

    cb->Get((float*)buffer, size*2);

    return Result::SUCCESS;
}

Result Channel::WriteStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;
    return Result::ERROR;
}

int Channel::readStream(airspy_transfer_t* transfer) {
    Channel* ctx = (Channel*)transfer->ctx;
    ctx->cb->Put((float*)transfer->samples, transfer->sample_count*2);
    return 0;
}

Result Channel::SetupStream() {
    if (stream.created || stream.running || !configured)
        return Result::ERROR_CHANNEL_NOT_READY;

    enum airspy_sample_type dtype;
    switch(config.dataFmt) {
        case Format::F32:
            dtype = AIRSPY_SAMPLE_FLOAT32_IQ;
            break;
        case Format::I16:
            dtype = AIRSPY_SAMPLE_INT16_IQ;
            break;
        case Format::I12:
            return Result::ERROR_FORMAT_NOT_SUPPORTED;
    }

    if (airspy_set_sample_type(fdn.device, dtype) != AIRSPY_SUCCESS) {
        return Result::ERROR_FORMAT_NOT_SUPPORTED;
    }

    cb = new CircularBuffer<float>(1024*1024*2);

    stream.created = true;
    return Result::SUCCESS;
}

Result Channel::DestroyStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    stream.created = false;
    return Result::SUCCESS;
}

Result Channel::StartStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    int result = airspy_start_rx(fdn.device, this->readStream, this);
    if (result != AIRSPY_SUCCESS) {
        return Result::ERROR_DEVICE_API;
    }


    stream.running = true;
    return Result::SUCCESS;
}

Result Channel::StopStream() {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (airspy_stop_rx(fdn.device) != AIRSPY_SUCCESS) {
        return Result::ERROR_DEVICE_API;
    }

    stream.running = false;
    return Result::SUCCESS;
}

Result Channel::GetFoundation(void* foundation) {
    *(Channel::Foundation*)foundation = this->fdn;
    return Result::SUCCESS;
}

Result Channel::GetConfig(Config* config) {
    *config = this->config;
    return Result::SUCCESS;
}

Result Channel::GetState(State* state) {
    *state = this->state;
    return Result::SUCCESS;
}

} // namespace Samurai::Airspy
