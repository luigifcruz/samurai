#ifndef SAMURAI_AIRSPY_CHANNEL_H
#define SAMURAI_AIRSPY_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <libairspy/airspy.h>

#include "samurai/base/channel.hpp"
#include "samurai/cbuffer.hpp"

namespace Samurai::Airspy {

class Channel : public Samurai::Channel {
    public:
        struct Foundation {
            uint id;
            uint index;
            struct airspy_device *device;
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
            bool created = false;
            bool running = false;
        };

        State state;
        Config config;
        Foundation fdn;
        Stream stream;
        CircularBuffer<float>* cb;

        bool configured;

        static int readStream(airspy_transfer_t*);
};

} // namespace Samurai::Airspy

#endif
