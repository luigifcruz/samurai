#include "samurai/limesdr/channel.hpp"

namespace Samurai::LimeSDR {

Channel::Channel(void* fdn_ptr, Config cfg) {
    this->fdn = *(Channel::Foundation*)fdn_ptr;
    this->config = cfg;
}

Channel::~Channel() {
    StopStream();
    DestroyStream();

    if (LMS_EnableChannel(fdn.device, getMode(config.mode), fdn.index, false) != 0) {
        std::cerr << "Can't disable channel." << std::endl;
    }
}

Result Channel::enable() {
    if (LMS_EnableChannel(fdn.device, getMode(config.mode), fdn.index, true) != 0) {
        return Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
    }
    return Result::SUCCESS;
}

Result Channel::update(State ns, State os, bool force) {
    if (ns.enableAGC != os.enableAGC || force) {

    }

    if (ns.frequency != os.frequency || force) {
        LMS_SetLOFrequency(fdn.device, getMode(config.mode), fdn.index, ns.frequency);
    }

    if (ns.manualGain != os.manualGain || force) {

    }

    return Result::SUCCESS;
}

Result Channel::readStream(void* buffer, size_t size, uint timeout_ms) {
    if (LMS_RecvStream(&stream_data, buffer, size, nullptr, timeout_ms) == -1) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::writeStream(void* buffer, size_t size, uint timeout_ms) {
    if (LMS_SendStream(&stream_data, buffer, size, nullptr, timeout_ms) == -1) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::setupStream() {
    stream_data.channel = fdn.index;
    stream_data.fifoSize = 1024*1024;
    stream_data.throughputVsLatency = 0.5;
    stream_data.isTx = getMode(config.mode);

    switch(config.dataFmt) {
        case Format::F32:
            stream_data.dataFmt = lms_stream_t::LMS_FMT_F32;
            break;
        case Format::I16:
            stream_data.dataFmt = lms_stream_t::LMS_FMT_I16;
            break;
        case Format::I12:
            stream_data.dataFmt = lms_stream_t::LMS_FMT_I12;
            break;
    }

    if (LMS_SetupStream(fdn.device, &stream_data) != 0) {
        return Result::ERROR_DEVICE_API;
    }

    return Result::SUCCESS;
}

Result Channel::destroyStream() {
    if (LMS_DestroyStream(fdn.device, &stream_data) != 0) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::startStream() {
    if (LMS_StartStream(&stream_data) != 0) {
        return Result::ERROR_DEVICE_API;
    }
    return Result::SUCCESS;
}

Result Channel::stopStream() {
    if (LMS_StopStream(&stream_data) != 0) {
        return Result::ERROR_DEVICE_API;
    }
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

Result Channel::GetFoundation(void* foundation) {
    *(Channel::Foundation*)foundation = this->fdn;
    return Result::SUCCESS;
}

} // namespace Samurai::LimeSDR
