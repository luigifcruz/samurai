#ifndef SAMURAI_DEVICE_H
#define SAMURAI_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include "samurai/types.hpp"
#include "samurai/base/channel.hpp"

namespace Samurai {

class Device {
    public:
        struct Config {
            float sampleRate = 10e6;
        };

        virtual ~Device() = default;
        virtual Result Enable(Config) = 0;

        virtual Result EnableChannel(Channel::Config, ChannelId*) = 0;
        virtual Result UpdateChannel(ChannelId, Channel::State) = 0;
        virtual Result DisableChannel(ChannelId) = 0;

        virtual Result StartStream() = 0;
        virtual Result StopStream() = 0;

        virtual Result ReadStream(ChannelId, void*, size_t, uint timeout_ms = 100) = 0;
        virtual Result WriteStream(ChannelId, void*, size_t, uint timeout_ms = 100) = 0;

        virtual DeviceId GetDeviceType() = 0;
        virtual uint GetMaxNumberOfChannels(Mode) = 0;
        virtual uint GetNumberOfChannels(Mode) = 0;
};

} // namespace Samurai

#endif
