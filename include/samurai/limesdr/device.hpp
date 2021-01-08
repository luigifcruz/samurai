#ifndef SAMURAI_LIMESDR_DEVICE_H
#define SAMURAI_LIMESDR_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <lime/LimeSuite.h>

#include "samurai/base/device.hpp"
#include "samurai/limesdr/channel.hpp"

namespace Samurai::LimeSDR {

class Device : public Samurai::Device {
    public:
        ~Device();

        DeviceId GetDeviceType();

    private:
        lms_device_t* device = nullptr;

    protected:
        Result enable();
        Result configure();
        Result createChannel(Channel::Config, ChannelId*);
};

} // namespace Samurai::LimeSDR

#endif

