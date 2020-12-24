#include "samurai.h"

namespace Samurai::LimeSDR {

Channel::Channel(void* fdn_ptr, Config cfg) {

    this->fdn = *(Channel::Foundation*)fdn_ptr;
    this->config = cfg;

    if (LMS_EnableChannel(fdn.device, getMode(config.mode), fdn.index, true) != 0) {
        std::cerr << "Can't enable channel." << std::endl;
        throw Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
    }

    this->configured = false;
}

Channel::~Channel() {
    StopStream();
    DestroyStream();

    if (LMS_EnableChannel(fdn.device, getMode(config.mode), fdn.index, false) != 0) {
        std::cerr << "Can't disable channel." << std::endl;
    }
}

Result Channel::GetFoundation(Foundation* foundation) {
    *foundation = this->fdn;
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

Result Channel::Update(State s, bool force) {
    if (s.enableAGC != state.enableAGC || force) {

    }

    if (s.frequency != state.frequency || force) {
        LMS_SetLOFrequency(fdn.device, getMode(config.mode), fdn.index, state.frequency);
    }

    if (s.manualGain != state.manualGain || force) {

    }

    LMS_SetAntenna(fdn.device, getMode(config.mode), fdn.index, LMS_PATH_LNAH);

    this->configured = true;
    this->state = s;

    return Result::SUCCESS;
}

Result Channel::ReadStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (LMS_RecvStream(&stream.data, buffer, size, nullptr, timeout_ms) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    return Result::SUCCESS;
}

Result Channel::WriteStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (LMS_SendStream(&stream.data, buffer, size, nullptr, timeout_ms) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    return Result::SUCCESS;
}

Result Channel::SetupStream() {
    if (stream.created || stream.running || !configured)
        return Result::ERROR_CHANNEL_NOT_READY;

    stream.data.channel = fdn.index;
    stream.data.fifoSize = 1024*1024;
    stream.data.throughputVsLatency = 0.5;
    stream.data.isTx = getMode(config.mode);

    switch(config.dataFmt) {
        case Format::F32:
            stream.data.dataFmt = lms_stream_t::LMS_FMT_F32;
            break;
        case Format::I16:
            stream.data.dataFmt = lms_stream_t::LMS_FMT_I16;
            break;
        case Format::I12:
            stream.data.dataFmt = lms_stream_t::LMS_FMT_I12;
            break;
    }

    if (LMS_SetupStream(fdn.device, &stream.data) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    stream.created = true;
    return Result::SUCCESS;
}

Result Channel::DestroyStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (LMS_DestroyStream(fdn.device, &stream.data) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    stream.created = false;
    return Result::SUCCESS;
}

Result Channel::StartStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (LMS_StartStream(&stream.data) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    stream.running = true;
    return Result::SUCCESS;
}

Result Channel::StopStream() {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    if (LMS_StopStream(&stream.data) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    stream.running = false;
    return Result::SUCCESS;
}

bool Channel::getMode(Mode m) {
    switch(m) {
        case Mode::RX: return LMS_CH_RX;
        case Mode::TX: return LMS_CH_TX;
        default:
            throw Result::ERROR_INVALID_DATA_TYPE;
    }
}

} // namespace Samurai::LimeSDR
