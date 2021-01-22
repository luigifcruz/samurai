#ifndef SAMURAI_AIRSPYHF_CHANNEL_H
#define SAMURAI_AIRSPYHF_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>
#include <unistd.h>

#include <libairspyhf/airspyhf.h>

#include "samurai/base/channel.hpp"
#include "samurai/base/cbuffer.hpp"

namespace Samurai::AirspyHF {

class Channel : public Samurai::Channel {
    public:
        struct Foundation {
            uint id;
            uint index;
            airspyhf_device_t *device;
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
        static int readCallback(airspyhf_transfer_t*);
};

} // namespace Samurai::AirspyHF
#endif
