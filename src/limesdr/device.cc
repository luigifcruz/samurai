#include "samurai/limesdr/device.hpp"

namespace Samurai::LimeSDR {

Device::~Device() {
    if (device != nullptr) {
        LMS_Close(device);
    }
}

DeviceId Device::GetDeviceType() {
    return DeviceId::LimeSDR;
}

Result Device::enable() {
    int n = 0;
    lms_info_str_t devices[16];

    if ((n = LMS_GetDeviceList(devices)) < 0) {
        return Result::ERROR_FAILED_TO_FIND_DEVICE;
    }

    if (LMS_Open(&device, devices[0], nullptr)) {
        return Result::ERROR_FAILED_TO_OPEN_DEVICE;
    }

    if (LMS_Init(device) != 0) {
        return Result::ERROR_FAILED_TO_INIT_DEVICE;
    }

    if ((n_channels[to_underlying(Mode::RX)] = LMS_GetNumChannels(device, LMS_CH_RX)) < 0) {
        LMS_Close(device);
        return Result::ERROR_FAILED_TO_FETCH_DATA;
    }

    if ((n_channels[to_underlying(Mode::TX)] = LMS_GetNumChannels(device, LMS_CH_TX)) < 0) {
        LMS_Close(device);
        return Result::ERROR_FAILED_TO_FETCH_DATA;
    }

    return Result::SUCCESS;
}

Result Device::configure() {
    auto res = Result::SUCCESS;

    if (LMS_SetSampleRate(device, config.sampleRate, 1) != 0) {
        res = Result::ERROR_SAMPLERATE_NOT_SUPPORTED;
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

} // namespace Samurai::LimeSDR
