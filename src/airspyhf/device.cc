#include "samurai/airspyhf/device.hpp"

namespace Samurai::AirspyHF {

Device::~Device() {
    if (device != nullptr) {
        airspyhf_close(device);
    }
}

DeviceId Device::GetDeviceType() {
    return DeviceId::AirspyHF;
}

Result Device::enable() {
    if (airspyhf_open(&device) != AIRSPYHF_SUCCESS) {
        return Result::ERROR_FAILED_TO_FIND_DEVICE;
    }

    n_channels[to_underlying(Mode::RX)] = 1;
    n_channels[to_underlying(Mode::TX)] = 0;

    return Result::SUCCESS;
}

Result Device::configure() {
    auto res = Result::SUCCESS;

    if (airspyhf_set_samplerate(device, config.sampleRate) != AIRSPYHF_SUCCESS) {
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

} // namespace Samurai::AirspyHF
