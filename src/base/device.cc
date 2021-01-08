#include "samurai/airspy/device.hpp"
#include "samurai/base/types.hpp"

namespace Samurai {

Result Device::Enable(Config cfg) {
    auto res = Result::SUCCESS;

    this->config = cfg;

    res = enable();
    ASSERT_RESULT(res);

    this->enabled = true;

exception:
    return res;
}

Result Device::EnableChannel(Channel::Config cfg, ChannelId* id) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (GetNumberOfChannels(cfg.mode) >= n_channels[to_underlying(cfg.mode)]) {
        std::cerr << "Maximum number of channels was exceeded." << std::endl;
        return Result::ERROR_MAX_NUMBER_OF_CHANNELS_REACHED;
    }

    return createChannel(cfg, id);
}

Result Device::UpdateChannel(ChannelId id, Channel::State state) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->Update(state);
}

Result Device::DisableChannel(ChannelId id) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    channels.erase(channels.begin() + id);
    return Result::SUCCESS;
}



Result Device::StartStream() {
    auto res = Result::SUCCESS;

    if (!enabled) {
        res = Result::ERROR_DEVICE_NOT_READY;
        ASSERT_RESULT(res);
    }

    res = configure();
    ASSERT_RESULT(res);

    for (const auto& channel : channels) {
        res = channel->SetupStream();
        ASSERT_RESULT(res);
    }

    for (const auto& channel : channels) {
        res = channel->StartStream();
        ASSERT_RESULT(res);
    }

exception:
    return res;
}

Result Device::StopStream() {
    Result res = Result::SUCCESS;

    if (!enabled) {
        res = Result::ERROR_DEVICE_NOT_READY;
        goto exception;
    }

    for (const auto& channel : channels) {
        res = channel->StopStream();
        ASSERT_RESULT(res);
    }

    for (const auto& channel : channels) {
        res = channel->DestroyStream();
        ASSERT_RESULT(res);
    }

exception:
    return res;
}

Result Device::ReadStream(ChannelId id, void* buffer, size_t size, uint timeout_ms) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->ReadStream(buffer, size, timeout_ms);
}

Result Device::WriteStream(ChannelId id, void* buffer, size_t size, uint timeout_ms) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->WriteStream(buffer, size, timeout_ms);
}

uint Device::GetMaxNumberOfChannels(Mode mode) {
    return n_channels[to_underlying(mode)];
}

uint Device::GetNumberOfChannels(Mode mode) {
    if (mode == Mode::NONE)
        return channels.size();

    uint matches = 0;
    for (const auto& channel : channels) {
        Channel::Config config;
        ASSERT_SUCCESS(channel->GetConfig(&config));
        if (config.mode == mode) {
            matches += 1;
        }
    }
    return matches;
}

size_t Device::BufferOccupancy(ChannelId id) {
    if (!enabled && channels.size() <= id)
        return 0;

    return channels.at(id)->BufferOccupancy();
}

Result Device::WaitBufferOccupancy(ChannelId id, size_t size) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->WaitBufferOccupancy(size);
}

} // namespace Samurai
