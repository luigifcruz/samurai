#include "samurai.h"

namespace Samurai::LimeSDR {

Channel::Channel(State ste, Config cfg) : state(ste), config(cfg) {
    if (LMS_EnableChannel(state.device, getMode(config.fdn.mode), state.index, true) != 0) {
        std::cerr << "Can't enable channel." << std::endl;
        throw Exceptions::CANT_CONFIGURE_DEVICE;
    }

    SetConfig(cfg, true);
}

Channel::~Channel() {
    _stopStream();
    _destroyStream();

    if (LMS_EnableChannel(state.device, getMode(config.fdn.mode), state.index, false) != 0) {
        std::cerr << "Can't disable channel." << std::endl;
    }
}

void Channel::SetConfig(Config cfg, bool force) {
    if (cfg.enableAGC != config.enableAGC || force) {

    }

    if (cfg.frequency != config.frequency || force) {
        LMS_SetLOFrequency(state.device, getMode(config.fdn.mode), state.index, cfg.frequency);
    }

    if (cfg.manualGain != config.manualGain || force) {

    }

    LMS_SetAntenna(state.device, getMode(config.fdn.mode), state.index, LMS_PATH_LNAH);

    this->config = cfg;
}

Channel::Config& Channel::GetConfig() {
    return config;
}

int Channel::ReadStream(float* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return -1;
    return LMS_RecvStream(&stream.data, buffer, size, nullptr, timeout_ms);
}

int Channel::WriteStream(float* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return -1;
    return LMS_SendStream(&stream.data, buffer, size, nullptr, timeout_ms);
}

int Channel::_setupStream() {
    if (stream.created || stream.running)
        return -1;

    stream.data.channel = state.index;
    stream.data.fifoSize = 1024*1024;
    stream.data.throughputVsLatency = 0.5;
    stream.data.isTx = getMode(config.fdn.mode);

    switch(config.fdn.dataFmt) {
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

    int res = LMS_SetupStream(state.device, &stream.data);
    if (res == 0)
        stream.created = true;
    return res;
}

int Channel::_destroyStream() {
    if (!stream.created || stream.running)
        return -1;

    int res = LMS_DestroyStream(state.device, &stream.data);
    if (res == 0)
        stream.created = false;
    return res;
}

int Channel::_startStream() {
    if (!stream.created || stream.running)
        return -1;

    int res = LMS_StartStream(&stream.data);
    if (res == 0)
        stream.running = true;
    return res;
}

int Channel::_stopStream() {
    if (!stream.created || !stream.running)
        return -1;

    int res = LMS_StopStream(&stream.data);
    if (res == 0)
        stream.running = false;
    return res;
}

Channel::State& Channel::_getState() {
    return state;
}

bool Channel::getMode(Mode m) {
    switch(m) {
        case Mode::RX: return LMS_CH_RX;
        case Mode::TX: return LMS_CH_TX;
        default:
            throw Exceptions::INVALID_TYPE;
    }
}

} // namespace Samurai::LimeSDR
