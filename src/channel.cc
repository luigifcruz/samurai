#include "samurai.h"
#include <lime/LimeSuite.h>

namespace Samurai::LimeSDR {

Channel::Channel(Mode mode, State& state, uint ch, uint id) : ch(ch), id(id), state(state) {
    switch(mode) {
        case MODE_RX:
            native_mode = LMS_CH_RX;
            break;
        case MODE_TX:
            native_mode = LMS_CH_TX;
            break;
        default:
            break;
    }

    stream.channel = ch;
    stream.fifoSize = 1024*1024;
    stream.throughputVsLatency = 0.5;
    stream.isTx = native_mode;
    stream.dataFmt = lms_stream_t::LMS_FMT_F32;

    if (LMS_EnableChannel(state.device, native_mode, ch, true) != 0) {
        std::cerr << "Can't enable channel." << std::endl;
        throw std::runtime_error("CANT_MEET_EXPECTATIONS");
    }
}

Channel::~Channel() {
    stopStream();
    destroyStream();

    if (LMS_EnableChannel(state.device, native_mode, ch, false) != 0) {
        std::cerr << "Can't disable channel." << std::endl;
    }
}

int Channel::setupStream() {
    if (stream_created || stream_running)
        return -1;

    int res = LMS_SetupStream(state.device, &stream);
    if (res == 0)
        stream_created = true;
    return res;
}

int Channel::destroyStream() {
    if (!stream_created || !stream_running)
        return -1;

    int res = LMS_DestroyStream(state.device, &stream);
    if (res == 0)
        stream_created = false;
    return res;
}

int Channel::startStream() {
    if (!stream_created || stream_running)
        return -1;

    int res = LMS_StartStream(&stream);
    if (res == 0)
        stream_running = true;
    return res;
}

int Channel::stopStream() {
    if (!stream_created || !stream_running)
        return -1;

    int res = LMS_StopStream(&stream);
    if (res == 0)
        stream_running = false;
    return res;
}

int Channel::ReadStream(float* buffer, size_t size, uint timeout_ms) {
    if (!stream_created || !stream_running)
        return -1;
    return LMS_RecvStream(&stream, buffer, size, nullptr, timeout_ms);
}

int Channel::WriteStream(float* buffer, size_t size, uint timeout_ms) {
    if (!stream_created || !stream_running)
        return -1;
    return LMS_SendStream(&stream, buffer, size, nullptr, timeout_ms);
}

float Channel::SetFrequency(float frequency) {
    LMS_SetLOFrequency(state.device, native_mode, ch, frequency);
    return GetFrequency();
}

float Channel::GetFrequency() {
    double frequency;
    LMS_GetLOFrequency(state.device, native_mode, ch, &frequency);
    LMS_SetNormalizedGain(state.device, native_mode, ch, 0.7);
    LMS_SetAntenna(state.device, native_mode, ch, LMS_PATH_LNAH);
    return frequency;
}

float Channel::SetGain(float gain) {
}

float Channel::GetGain() {
}

bool Channel::SetAGC(bool status) {
}

bool Channel::SetCache(bool status) {
}

Mode Channel::GetMode() {
    return mode;
}

bool Channel::IsRunning() {
    return stream_created | stream_running;
}

uint Channel::GetId() {
    return id;
}

uint Channel::GetChannel() {
    return ch;
}

} // namespace Samurai::LimeSDR
