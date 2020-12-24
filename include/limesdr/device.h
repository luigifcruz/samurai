#ifndef SAMURAI_LIMESDR_DEVICE_H
#define SAMURAI_LIMESDR_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include <lime/LimeSuite.h>

#include "base/device.h"
#include "base/channel.h"
#include "limesdr/channel.h"

namespace Samurai::LimeSDR {

class Device : public Samurai::Device {
    public:
        Device(Device::Config);
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

