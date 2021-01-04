#include "samurai/airspy/device.hpp"

namespace Samurai::Airspy {

Device::Device() {
    enabled = false;
}

Device::~Device() {
    channels.clear();

    if (device != nullptr) {
        airspy_close(device);
    }
}

Result Device::Enable(Config cfg) {
    this->config = cfg;

    if (airspy_open(&device) != AIRSPY_SUCCESS) {
        return Result::ERROR_FAILED_TO_FIND_DEVICE;
    }

    n_channels[to_underlying(Mode::RX)] = 1;
    n_channels[to_underlying(Mode::TX)] = 0;

    this->enabled = true;
    return Result::SUCCESS;
}

Result Device::EnableChannel(Channel::Config cfg, ChannelId* id) {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    if (GetNumberOfChannels(cfg.mode) >= n_channels[to_underlying(cfg.mode)]) {
        std::cerr << "Maximum number of channels was exceeded." << std::endl;
        return Result::ERROR_MAX_NUMBER_OF_CHANNELS_REACHED;
    }

    Channel::Foundation channelFoundation{};
    channelFoundation.device = device;
    channelFoundation.index = GetNumberOfChannels(cfg.mode);
    channelFoundation.id = channels.size();

    auto channel = std::make_shared<Channel>((void*)&channelFoundation, cfg);
    channels.push_back(channel);
    *id = channels.size() - 1;

    return Result::SUCCESS;
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
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    Result err = Result::SUCCESS;

    if (airspy_set_samplerate(device, config.sampleRate) != AIRSPY_SUCCESS) {
        err = Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        goto exception;
    }

    for (const auto& channel : channels) {
        err = channel->SetupStream();
        if (err != Result::SUCCESS) goto exception;
    }

    for (const auto& channel : channels) {
        err = channel->StartStream();
        if (err != Result::SUCCESS) goto exception;
    }

exception:
    return err;
}

Result Device::StopStream() {
    if (!enabled)
        return Result::ERROR_DEVICE_NOT_READY;

    Result err = Result::SUCCESS;

    for (const auto& channel : channels) {
        err = channel->StopStream();
        if (err != Result::SUCCESS) goto exception;
    }

    for (const auto& channel : channels) {
        err = channel->DestroyStream();
        if (err != Result::SUCCESS) goto exception;
    }

exception:
    return err;
}

Result Device::ReadStream(ChannelId id, void* buffer, size_t size, uint timeout_ms) {
    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->ReadStream(buffer, size, timeout_ms);
}

Result Device::WriteStream(ChannelId id, void* buffer, size_t size, uint timeout_ms) {
    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->WriteStream(buffer, size, timeout_ms);
}

DeviceId Device::GetDeviceType() {
    return DeviceId::LimeSDR;
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

uint Device::GetMaxNumberOfChannels(Mode mode) {
    return n_channels[to_underlying(mode)];
}

Channel::State Device::getChannelState(ChannelId id) {
    Channel::State state;
    ASSERT_SUCCESS(channels.at(id)->GetState(&state));
    return state;
}

} // namespace Samurai::Airspy
