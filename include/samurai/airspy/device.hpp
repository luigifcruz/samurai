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
        Device();
        ~Device();
        Result Enable(Config);

        Result EnableChannel(Channel::Config, ChannelId*);
        Result UpdateChannel(ChannelId, Channel::State);
        Result DisableChannel(ChannelId);

        Result StartStream();
        Result StopStream();

        Result ReadStream(ChannelId, void*, size_t, uint timeout_ms = 100);
        Result WriteStream(ChannelId, void*, size_t, uint timeout_ms = 100);

        DeviceId GetDeviceType();
        uint GetMaxNumberOfChannels(Mode);
        uint GetNumberOfChannels(Mode);

    private:
        bool enabled;
        Config config;
        uint n_channels[8] = {};
        struct airspy_device *device;

        std::vector<std::shared_ptr<Channel>> channels;

        Channel::State getChannelState(ChannelId);
};

} // namespace Samurai::Airspy

#endif

