#ifndef SAMURAI_H
#define SAMURAI_H

#include <memory>

#include "samurai/types.h"
#include "config.h"

#include "samurai/base/device.h"
#include "samurai/base/channel.h"

#ifdef SAMURAI_LIMESDR_ENABLED
#include "samurai/limesdr/device.h"
#include "samurai/limesdr/channel.h"
#endif

namespace Samurai {

inline std::vector<DeviceId> AvailableDevices = {
    #ifdef SAMURAI_LIMESDR_ENABLED
    DeviceId::LimeSDR,
    #endif
};

inline std::unique_ptr<Device> GetDeviceHandler(DeviceId device) {
    switch(device) {
    #ifdef SAMURAI_LIMESDR_ENABLED
        case DeviceId::LimeSDR:
            return std::make_unique<LimeSDR::Device>();
    #endif
        default:
            throw Result::ERROR_DEVICE_NOT_SUPPORTED;
    }
}

} // namespace Samurai

#endif
