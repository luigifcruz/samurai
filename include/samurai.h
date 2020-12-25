#ifndef SAMURAI_H
#define SAMURAI_H

#include <memory>

#include "config.h"

#ifdef SAMURAI_LIMESDR_ENABLED
#include "limesdr/device.h"
#include "limesdr/channel.h"
#endif

namespace Samurai {

inline std::vector<DeviceId> SupportedDevices = {
    #ifdef SAMURAI_LIMESDR_ENABLED
    DeviceId::LimeSDR,
    #endif
};

inline std::shared_ptr<Device> GetDeviceHandler(DeviceId device) {
    switch(device) {
    #ifdef SAMURAI_LIMESDR_ENABLED
        case DeviceId::LimeSDR:
            return std::make_shared<LimeSDR::Device>();
    #endif
        default:
            throw Result::ERROR_DEVICE_NOT_SUPPORTED;
    }
}

} // namespace Samurai

#endif
