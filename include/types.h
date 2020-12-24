#ifndef SAMURAI_TYPES_H
#define SAMURAI_TYPES_H

#include <iostream>
#include <type_traits>

#include "tools/magic_enum.hpp"

namespace Samurai::LimeSDR {

template <typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

enum struct Mode {
    NONE = 0 << 0,
    RX   = 1 << 0,
    TX   = 1 << 1,
};

enum struct Result {
    SUCCESS,
    ERROR,
    ERROR_DEVICE_NOT_READY,
    ERROR_CHANNEL_NOT_READY,
    ERROR_MAX_NUMBER_OF_CHANNELS_REACHED,
    ERROR_FAILED_TO_FIND_DEVICE,
    ERROR_FAILED_TO_FIND_CHANNEL,
    ERROR_FAILED_TO_OPEN_DEVICE,
    ERROR_FAILED_TO_INIT_DEVICE,
    ERROR_FAILED_TO_CONFIGURE_DEVICE,
    ERROR_FAILED_TO_FETCH_DATA,
    ERROR_INVALID_DATA_TYPE,
    ERROR_DEVICE_API,
};

enum struct Format {
    F32,
    I16,
    I12,
};

typedef uint ChannelId;

#define ASSERT_SUCCESS(result) { \
    if (result != Result::SUCCESS) { \
        std::cerr << "Samurai encountered an exception (" <<  magic_enum::enum_name(result) << ") in line " \
            << __LINE__ << " of file " << __FILE__ << "." << std::endl; \
        throw result; \
    } \
}

} // namespace Samurai::LimeSDR

#endif
