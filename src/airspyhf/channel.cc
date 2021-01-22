#include "samurai/airspyhf/channel.hpp"

namespace Samurai::AirspyHF {

Channel::Channel(void* fdn_ptr, Config cfg) {
    this->fdn = *(Channel::Foundation*)fdn_ptr;
    this->config = cfg;
}

Channel::~Channel() {
    StopStream();
    DestroyStream();
}

Result Channel::enable() {
    return Result::SUCCESS;
}

Result Channel::update(State ns, State os, bool force) {
    if (ns.enableAGC != os.enableAGC || force) {
        int res = 0;
        res += airspyhf_set_hf_agc(fdn.device, ns.enableAGC ? 1 : 0);
        res += airspyhf_set_hf_lna(fdn.device, ns.enableAGC ? 1 : 0);
        if (res != 0) {
            return Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        }
    }

    if (ns.frequency != os.frequency || force) {
        if (airspyhf_set_freq(fdn.device, static_cast<uint32_t>(ns.frequency)) != AIRSPYHF_SUCCESS) {
            return Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        }
    }

    if ((ns.manualGain != os.manualGain && !ns.enableAGC) || force) {

    }

    if (stream.running)
        usleep(15000);

    if (stream.created)
        this->cb->Reset();

    return Result::SUCCESS;
}

Result Channel::readStream(void* buffer, size_t size, uint timeout_ms) {
    return cb->Get((float*)buffer, size * 2);
}

Result Channel::writeStream(void* buffer, size_t size, uint timeout_ms) {
    return Result::ERROR;
}

Result Channel::setupStream() {
    if (config.dataFmt != Format::F32) {
        return Result::ERROR_FORMAT_NOT_SUPPORTED;
    }

    return Result::SUCCESS;
}

Result Channel::destroyStream() {
    return Result::SUCCESS;
}

Result Channel::startStream() {
    int result = airspyhf_start(fdn.device, this->readCallback, this);
    if (result != AIRSPYHF_SUCCESS) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::stopStream() {
    if (airspyhf_stop(fdn.device) != AIRSPYHF_SUCCESS) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::GetFoundation(void* foundation) {
    *(Channel::Foundation*)foundation = this->fdn;
    return Result::SUCCESS;
}

int Channel::readCallback(airspyhf_transfer_t* transfer) {
    Channel* ctx = (Channel*)transfer->ctx;
    size_t size = transfer->sample_count * 2;

    if (ctx->cb->Put((float*)transfer->samples, size) != Result::SUCCESS) {
        return -1;
    }

    return 0;
}

} // namespace Samurai::AirspyHF
