#ifndef SAMURAI_TYPES_H
#define SAMURAI_TYPES_H

#include <iostream>
#include <type_traits>

namespace Samurai {

#ifdef __llvm__
typedef unsigned int uint;
#endif

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
    SUCCESS = 0,
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
    ERROR_DEVICE_NOT_SUPPORTED,
    ERROR_FORMAT_NOT_SUPPORTED,
    ERROR_CANT_ALLOCATE_MEMORY,
    ERROR_TIMEOUT,
    ERROR_BEYOND_CAPACITY,
    ERROR_SAMPLERATE_NOT_SUPPORTED,
};

enum struct Format {
    F32,
    I16,
    I12,
};

enum struct DeviceId {
    LimeSDR  = 1 << 0,
    Airspy   = 1 << 1,
    AirspyHF = 1 << 2,
};

typedef uint ChannelId;

void print_error(Result, const char*, int, const char*);

#ifndef SAMURAI_CHECK_THROW
#define SAMURAI_CHECK_THROW(result) { \
    if (result != Samurai::Result::SUCCESS) { \
        print_error(result, __PRETTY_FUNCTION__, __LINE__, __FILE__); \
        throw result; \
    } \
}
#endif

#ifndef CHECK
#define CHECK(result) { \
    if (result != Samurai::Result::SUCCESS) { \
        print_error(result, __PRETTY_FUNCTION__, __LINE__, __FILE__); \
        return result; \
    } \
}
#endif

#define ASSERT_RESULT(result) { \
    if (result != Result::SUCCESS) { \
        goto exception; \
    } \
}

#define MIN(a,b) (((a)<(b))?(a):(b))

} // namespace Samurai

#endif
