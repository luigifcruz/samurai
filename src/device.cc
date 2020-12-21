#include "samurai.h"

namespace Samurai::LimeSDR {

Device::Device() {
    int n = 0;
    lms_info_str_t devices[16];

    if ((n = LMS_GetDeviceList(devices)) < 0) {
        std::cerr << "No LimeSDR devices found." << std::endl;
        throw std::runtime_error("NO_DEVICE_FOUND");
    }

    if (LMS_Open(&state.device, devices[0], nullptr)) {
        std::cerr << "Can't open LimeSDR device." << std::endl;
        throw std::runtime_error("CANT_OPEN_DEVICE");
    }

    if (LMS_Init(state.device) != 0) {
        std::cerr << "Can't init LimeSDR device" << std::endl;
        throw std::runtime_error("CANT_OPEN_DEVICE");
    }

    if ((n_channels[MODE_RX] = LMS_GetNumChannels(state.device, LMS_CH_RX)) < 0) {
        std::cerr << "Can't get number of RX channels." << std::endl;
        LMS_Close(state.device);
        throw std::runtime_error("CANT_FETCH_DATA");
    }

    if ((n_channels[MODE_TX] = LMS_GetNumChannels(state.device, LMS_CH_TX)) < 0) {
        std::cerr << "Can't get number of TX channels." << std::endl;
        LMS_Close(state.device);
        throw std::runtime_error("CANT_FETCH_DATA");
    }
}

Device::~Device() {
    channels.clear();
    if (state.device != nullptr) {
        LMS_Close(state.device);
    }
}

Channel& Device::EnableChannel(Mode mode) {
    if (GetNumberOfChannels(mode) >= n_channels[mode]) {
        std::cerr << "Maximum number of channels was exceeded." << std::endl;
        throw std::runtime_error("CANT_MEET_EXPECTATIONS");
    }

    uint id = channels.size();
    uint ch_n = GetNumberOfChannels(mode);

    auto channel = std::make_shared<Channel>(mode, state, ch_n, id);
    channels.push_back(channel);
    return *channel;
}

bool Device::DisableChannel(Channel& channel) {
    if (channels.size() >= channel.GetId()) {
        channels.erase(channels.begin() + channel.GetId());
        return true;
    }
    return false;
}

int Device::SetSamplerate(float samplerate) {
    return LMS_SetSampleRate(state.device, samplerate, 1);
}

int Device::StartStreams() {
    int err = 0;

    for (const auto& channel : channels) {
        err = channel->setupStream();
        if (err != 0) return err;
    }

    for (const auto& channel : channels) {
        channel->startStream();
        if (err != 0) return err;
    }

    return err;
}

int Device::StopStreams() {
    int err = 0;

    for (const auto& channel : channels) {
        err = channel->stopStream();
        if (err != 0) return err;
    }

    for (const auto& channel : channels) {
        channel->destroyStream();
        if (err != 0) return err;
    }

    return err;
}

uint Device::GetNumberOfChannels(Mode mode) {
    if (mode == MODE_NONE)
        return channels.size();

    uint matches = 0;
    for (const auto& channel : channels) {
        if (channel->GetMode() == mode)
            matches += 1;
    }
    return matches;
}

uint Device::GetMaxNumberOfChannels(Mode mode) {
    return n_channels[mode];
}

bool Device::IsRunning() {
    for (const auto& channel : channels) {
        if (channel->IsRunning())
            return true;
    }
    return false;
}

} // namespace Samurai::LimeSDR
