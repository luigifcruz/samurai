#include "samurai/airspy/device.hpp"

namespace Samurai::Airspy {

Device::~Device() {
    if (device != nullptr) {
        airspy_close(device);
    }
}

DeviceId Device::GetDeviceType() {
    return DeviceId::Airspy;
}

Result Device::enable() {
    if (airspy_open(&device) != AIRSPY_SUCCESS) {
        return Result::ERROR_FAILED_TO_FIND_DEVICE;
    }

    n_channels[to_underlying(Mode::RX)] = 1;
    n_channels[to_underlying(Mode::TX)] = 0;

    return Result::SUCCESS;
}

Result Device::configure() {
    auto res = Result::SUCCESS;

    if (airspy_set_samplerate(device, config.sampleRate) != AIRSPY_SUCCESS) {
        res = Result::ERROR_FAILED_TO_CONFIGURE_DEVICE;
        goto exception;
    }

exception:
    return res;
}

Result Device::createChannel(Channel::Config cfg, ChannelId* id) {
    Channel::Foundation channelFoundation{};

    channelFoundation.device = device;
    channelFoundation.index = GetNumberOfChannels(cfg.mode);
    channelFoundation.id = channels.size();

    auto channel = std::make_shared<Channel>((void*)&channelFoundation, cfg);
    channels.push_back(channel);
    *id = channels.size() - 1;

    return Result::SUCCESS;
}

} // namespace Samurai::Airspy
