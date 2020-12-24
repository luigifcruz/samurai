#include "samurai.h"

namespace Samurai::LimeSDR {

Device::Device(Device::Config cfg) : config(cfg) {
    int n = 0;
    lms_info_str_t devices[16];

    if ((n = LMS_GetDeviceList(devices)) < 0) {
        ASSERT_SUCCESS(Result::ERROR_FAILED_TO_FIND_DEVICE);
    }

    if (LMS_Open(&device, devices[0], nullptr)) {
        ASSERT_SUCCESS(Result::ERROR_FAILED_TO_OPEN_DEVICE);
    }

    if (LMS_Init(device) != 0) {
        ASSERT_SUCCESS(Result::ERROR_FAILED_TO_INIT_DEVICE);
    }

    if ((n_channels[to_underlying(Mode::RX)] = LMS_GetNumChannels(device, LMS_CH_RX)) < 0) {
        LMS_Close(device);
        ASSERT_SUCCESS(Result::ERROR_FAILED_TO_FETCH_DATA);
    }

    if ((n_channels[to_underlying(Mode::TX)] = LMS_GetNumChannels(device, LMS_CH_TX)) < 0) {
        LMS_Close(device);
        ASSERT_SUCCESS(Result::ERROR_FAILED_TO_FETCH_DATA);
    }
}

Device::~Device() {
    channels.clear();

    if (device != nullptr) {
        LMS_Close(device);
    }
}

Result Device::EnableChannel(Channel::Config cfg, ChannelId* id) {
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
    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    return channels.at(id)->Update(state);
}

Result Device::DisableChannel(ChannelId id) {
    if (channels.size() <= id)
        return Result::ERROR_FAILED_TO_FIND_CHANNEL;

    channels.erase(channels.begin() + id);
    return Result::SUCCESS;
}

Result Device::StartStream() {
    Result err = Result::SUCCESS;

    int res;
    if (( res = LMS_SetSampleRate(device, config.sampleRate, 1)) != 0) {
        err = Result::ERROR;
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

} // namespace Samurai::LimeSDR
