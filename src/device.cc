#include "samurai.h"

namespace Samurai::LimeSDR {

Device::Device(Device::Config cfg) : config(cfg) {
    int n = 0;
    lms_info_str_t devices[16];

    if ((n = LMS_GetDeviceList(devices)) < 0) {
        std::cerr << "No LimeSDR devices found." << std::endl;
        throw Exceptions::CANT_FIND_DEVICE;
    }

    if (LMS_Open(&device, devices[0], nullptr)) {
        std::cerr << "Can't open LimeSDR device." << std::endl;
        throw Exceptions::CANT_OPEN_DEVICE;
    }

    if (LMS_Init(device) != 0) {
        std::cerr << "Can't init LimeSDR device" << std::endl;
        throw Exceptions::CANT_CONFIGURE_DEVICE;
    }

    if ((n_channels[to_underlying(Mode::RX)] = LMS_GetNumChannels(device, LMS_CH_RX)) < 0) {
        std::cerr << "Can't get number of RX channels." << std::endl;
        LMS_Close(device);
        throw Exceptions::CANT_FETCH_DATA;
    }

    if ((n_channels[to_underlying(Mode::TX)] = LMS_GetNumChannels(device, LMS_CH_TX)) < 0) {
        std::cerr << "Can't get number of TX channels." << std::endl;
        LMS_Close(device);
        throw Exceptions::CANT_FETCH_DATA;
    }
}

Device::~Device() {
    channels.clear();

    if (device != nullptr) {
        LMS_Close(device);
    }
}

Channel& Device::EnableChannel(Channel::Config cfg) {
    if (GetNumberOfChannels(cfg.fdn.mode) >= n_channels[to_underlying(cfg.fdn.mode)]) {
        std::cerr << "Maximum number of channels was exceeded." << std::endl;
        throw Exceptions::CANT_CONFIGURE_DEVICE;
    }

    Channel::State channelState{};
    channelState.device = device;
    channelState.index = GetNumberOfChannels(cfg.fdn.mode);
    channelState.id = channels.size();
    auto channel = std::make_shared<Channel>(channelState, cfg);

    channels.push_back(channel);
    return *channel;
}

bool Device::DisableChannel(Channel& channel) {
    if (channels.size() >= channel._getState().id) {
        channels.erase(channels.begin() + channel._getState().id);
        return true;
    }
    return false;
}

int Device::StartStream() {
    int err = 0;

    err = LMS_SetSampleRate(device, config.sampleRate, 1);
    if (err != 0) goto exception;

    for (const auto& channel : channels) {
        err = channel->_setupStream();
        if (err != 0) goto exception;
    }

    for (const auto& channel : channels) {
        err = channel->_startStream();
        if (err != 0) goto exception;
    }

exception:

    return err;
}

int Device::StopStream() {
    int err = 0;

    for (const auto& channel : channels) {
        err = channel->_stopStream();
        if (err != 0) goto exception;
    }

    for (const auto& channel : channels) {
        err = channel->_destroyStream();
        if (err != 0) goto exception;
    }

exception:

    return err;
}

uint Device::GetNumberOfChannels(Mode mode) {
    if (mode == Mode::NONE)
        return channels.size();

    uint matches = 0;
    for (const auto& channel : channels) {
        if (channel->GetConfig().fdn.mode == mode)
            matches += 1;
    }
    return matches;
}

uint Device::GetMaxNumberOfChannels(Mode mode) {
    return n_channels[to_underlying(mode)];
}

} // namespace Samurai::LimeSDR
