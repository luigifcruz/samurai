#ifndef SAMURAI_TYPES_H
#define SAMURAI_TYPES_H

#include <type_traits>

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

enum struct Exceptions {
    CANT_FIND_DEVICE,
    CANT_OPEN_DEVICE,
    CANT_CONFIGURE_DEVICE,
    CANT_FETCH_DATA,
    INVALID_TYPE,
    UNSPECIFIED_ERROR,
};

enum struct Format {
    F32,
    I16,
    I12,
};

} // namespace Samurai::LimeSDR

#endif
