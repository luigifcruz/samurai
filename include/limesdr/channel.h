#ifndef SAMURAI_LIMESDR_CHANNEL_H
#define SAMURAI_LIMESDR_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <lime/LimeSuite.h>

#include "base/channel.h"

namespace Samurai::LimeSDR {

class Channel : public Samurai::Channel {
    public:
        struct Foundation {
            uint id;
            uint index;
            lms_device_t* device;
        };

        Channel(void*, Channel::Config);
        ~Channel();

        Result GetFoundation(void*);
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

} // namespace Samurai::LimeSDR

#endif
