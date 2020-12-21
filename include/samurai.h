#ifndef SAMURAI_H
#define SAMURAI_H

#include "lime/LimeSuite.h"

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

namespace Samurai::LimeSDR {

enum Mode : uint8_t {
    MODE_NONE = 0 << 0,
    MODE_RX   = 1 << 0,
    MODE_TX   = 1 << 1,
};

struct State {
    lms_device_t* device = nullptr;
};

class Channel {
    private:
        uint ch;
        uint id;
        Mode mode;
        State& state;
        bool native_mode;
        lms_stream_t stream;

        bool stream_created = false;
        bool stream_running = false;

    public:
        Channel(Mode, State&, uint, uint);
        ~Channel();

        uint GetId();
        uint GetChannel();
        Mode GetMode();
        bool IsRunning();

        float SetFrequency(float);
        float GetFrequency();

        float SetGain(float);
        float GetGain();

        bool SetAGC(bool);
        bool SetCache(bool);

        int ReadStream(float*, size_t, uint);
        int WriteStream(float*, size_t, uint);

        int setupStream();
        int destroyStream();
        int startStream();
        int stopStream();
};

class Device {
    private:
        State state;
        uint n_channels[8] = {};
        std::vector<std::shared_ptr<Channel>> channels;

    public:
        Device();
        ~Device();

        Channel& EnableChannel(Mode);
        bool DisableChannel(Channel&);

        int StartStreams();
        int StopStreams();

        int SetSamplerate(float);

        uint GetMaxNumberOfChannels(Mode);
        uint GetNumberOfChannels(Mode);

        bool IsRunning();
};

} // namespace Samurai::LimeSDR

#endif
