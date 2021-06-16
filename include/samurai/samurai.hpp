#ifndef SAMURAI_H
#define SAMURAI_H

#include <memory>

#include "samurai/base/cbuffer.hpp"
#include "samurai/base/type.hpp"
#include "config.hpp"

#include "samurai/base/device.hpp"
#include "samurai/base/channel.hpp"

#ifdef SAMURAI_LIMESDR_ENABLED
#include "samurai/limesdr/device.hpp"
#include "samurai/limesdr/channel.hpp"
#endif

#ifdef SAMURAI_AIRSPY_ENABLED
#include "samurai/airspy/device.hpp"
#include "samurai/airspy/channel.hpp"
#endif

#ifdef SAMURAI_AIRSPYHF_ENABLED
#include "samurai/airspyhf/device.hpp"
#include "samurai/airspyhf/channel.hpp"
#endif

namespace Samurai {

inline std::vector<DeviceId> AvailableDevices = {
    #ifdef SAMURAI_LIMESDR_ENABLED
    DeviceId::LimeSDR,
    #endif
    #ifdef SAMURAI_AIRSPY_ENABLED
    DeviceId::Airspy,
    #endif
    #ifdef SAMURAI_AIRSPYHF_ENABLED
    DeviceId::AirspyHF,
    #endif
};

inline std::unique_ptr<Device> GetDeviceHandler(DeviceId device) {
    switch(device) {
    #ifdef SAMURAI_LIMESDR_ENABLED
        case DeviceId::LimeSDR:
            return std::make_unique<LimeSDR::Device>();
    #endif
    #ifdef SAMURAI_AIRSPY_ENABLED
        case DeviceId::Airspy:
            return std::make_unique<Airspy::Device>();
    #endif
    #ifdef SAMURAI_AIRSPYHF_ENABLED
        case DeviceId::AirspyHF:
            return std::make_unique<AirspyHF::Device>();
    #endif
        default:
            SAMURAI_CHECK_THROW(Result::ERROR_DEVICE_NOT_SUPPORTED);
    }
}

} // namespace Samurai

#endif
