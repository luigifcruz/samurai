#ifndef SAMURAI_H
#define SAMURAI_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <lime/LimeSuite.h>

#include "types.h"

namespace Samurai::LimeSDR {

class Channel {
    public:
        struct Foundation {
            uint id;
            uint index;
            lms_device_t* device;
        };

        struct Config {
            Mode mode;
            Format dataFmt;
            bool calibrate;
        };

        struct State {
            float frequency;
            float manualGain;
            bool enableAGC;
        };

        Channel(void*, Config);
        ~Channel();

        Result GetFoundation(Foundation*);
        Result GetConfig(Config*);
        Result GetState(State*);

        Result Update(State, bool force=false);

        Result ReadStream(void*, size_t, uint);
        Result WriteStream(void*, size_t, uint);

        Result SetupStream();
        Result DestroyStream();
        Result StartStream();
        Result StopStream();

    private:
        struct Stream {
            lms_stream_t data;
            bool created = false;
            bool running = false;
        };

        State state;
        Config config;
        Foundation fdn;
        Stream stream;

        bool configured;

        bool getMode(Mode);
};


class Device {
    public:
        struct Config {
            float sampleRate;
        };

        Device(Config);
        ~Device();

        Result EnableChannel(Channel::Config, ChannelId*);
        Result UpdateChannel(ChannelId, Channel::State);
        Result DisableChannel(ChannelId);

        Result StartStream();
        Result StopStream();

        Result ReadStream(ChannelId, void*, size_t, uint timeout_ms = 100);
        Result WriteStream(ChannelId, void*, size_t, uint timeout_ms = 100);

        uint GetMaxNumberOfChannels(Mode);
        uint GetNumberOfChannels(Mode);

    private:
        Config config;
        uint n_channels[8] = {};
        lms_device_t* device = nullptr;

        std::vector<std::shared_ptr<Channel>> channels;

        Channel::State getChannelState(ChannelId);
};

} // namespace Samurai::LimeSDR

#endif
