#ifndef SAMURAI_AIRSPYHF_DEVICE_H
#define SAMURAI_AIRSPYHF_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <libairspyhf/airspyhf.h>

#include "samurai/base/device.hpp"
#include "samurai/airspyhf/channel.hpp"

namespace Samurai::AirspyHF {

class Device : public Samurai::Device {
    public:
        ~Device();

        DeviceId GetDeviceType();

    private:
        airspyhf_device_t *device;

    protected:
        Result enable();
        Result configure();
        Result createChannel(Channel::Config, ChannelId*);
};

} // namespace Samurai::AirspyHF

#endif

