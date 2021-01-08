#ifndef SAMURAI_LIMESDR_CHANNEL_H
#define SAMURAI_LIMESDR_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <lime/LimeSuite.h>

#include "samurai/base/channel.hpp"

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

    protected:
        Result enable();
        Result update(State, State, bool);

        Result readStream(void*, size_t, uint);
        Result writeStream(void*, size_t, uint);

        Result setupStream();
        Result destroyStream();
        Result startStream();
        Result stopStream();

    private:
        Foundation fdn;
        lms_stream_t stream_data;
        bool getMode(Mode);
};

} // namespace Samurai::LimeSDR

#endif
