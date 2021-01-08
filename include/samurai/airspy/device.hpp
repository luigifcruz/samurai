#ifndef SAMURAI_AIRSPYONE_DEVICE_H
#define SAMURAI_AIRSPYONE_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <libairspy/airspy.h>

#include "samurai/base/device.hpp"
#include "samurai/airspy/channel.hpp"

namespace Samurai::Airspy {

class Device : public Samurai::Device {
    public:
        ~Device();

        DeviceId GetDeviceType();

    private:
        struct airspy_device *device;

    protected:
        Result enable();
        Result configure();
        Result createChannel(Channel::Config, ChannelId*);
};

} // namespace Samurai::Airspy

#endif

