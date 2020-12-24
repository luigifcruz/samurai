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
        struct State {
            uint id;
            uint index;
            lms_device_t* device;
        };

        struct Config {
            struct Foundation {
                Mode mode;
                Format dataFmt;
                bool calibrate;
            };

            Foundation fdn;
            float frequency;
            float manualGain;
            bool enableAGC;
        };

        Channel(State, Config);
        ~Channel();

        void SetConfig(Config, bool force=false);
        Config& GetConfig();

        int ReadStream(float*, size_t, uint);
        int WriteStream(float*, size_t, uint);

        int _setupStream();
        int _destroyStream();
        int _startStream();
        int _stopStream();
        State& _getState();

    private:
        struct Stream {
            lms_stream_t data;
            bool created = false;
            bool running = false;
        };

        State state;
        Config config;
        Stream stream;

        bool getMode(Mode);
};

class Device {
    public:
        struct Config {
            float sampleRate;
        };

        Device(Config);
        ~Device();

        Channel& EnableChannel(Channel::Config);
        bool DisableChannel(Channel&);

        int StartStream();
        int StopStream();

        uint GetMaxNumberOfChannels(Mode);
        uint GetNumberOfChannels(Mode);

    private:
        Config config;
        uint n_channels[8] = {};
        lms_device_t* device = nullptr;
        std::vector<std::shared_ptr<Channel>> channels;
};

} // namespace Samurai::LimeSDR

#endif
