#include "samurai/base/type.hpp"
#include "samurai/tools/magic_enum.hpp"

namespace Samurai {

void print_error(Result res, const char* func, int line, const char* file) {
    std::cerr << "Render encountered an exception (" <<  magic_enum::enum_name(res) << ") in " \
        << func << " in line " << line << " of file " << file << "." << std::endl; \
}

} // namespace Samurai
