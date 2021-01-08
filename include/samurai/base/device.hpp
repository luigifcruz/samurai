#ifndef SAMURAI_DEVICE_H
#define SAMURAI_DEVICE_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include "samurai/base/types.hpp"
#include "samurai/base/channel.hpp"

namespace Samurai {

class Device {
    public:
        struct Config {
            float sampleRate = 10e6;
        };

        virtual ~Device() = default;
        virtual DeviceId GetDeviceType() = 0;

        Result Enable(Config);

        Result EnableChannel(Channel::Config, ChannelId*);
        Result UpdateChannel(ChannelId, Channel::State);
        Result DisableChannel(ChannelId);

        Result StartStream();
        Result StopStream();

        Result ReadStream(ChannelId, void*, size_t, uint timeout_ms = 100);
        Result WriteStream(ChannelId, void*, size_t, uint timeout_ms = 100);

        uint GetMaxNumberOfChannels(Mode);
        uint GetNumberOfChannels(Mode);

        size_t BufferOccupancy(ChannelId);
        Result WaitBufferOccupancy(ChannelId, size_t);

    protected:
        Config config;
        bool enabled = false;
        uint n_channels[8] = {};
        std::vector<std::shared_ptr<Channel>> channels;

        virtual Result enable() = 0;
        virtual Result configure() = 0;
        virtual Result createChannel(Channel::Config, ChannelId*) = 0;
};

} // namespace Samurai

#endif
