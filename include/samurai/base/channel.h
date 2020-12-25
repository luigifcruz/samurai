#ifndef SAMURAI_CHANNEL_H
#define SAMURAI_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include "samurai/types.h"

namespace Samurai {

class Channel {
    public:
        struct Config {
            Mode mode = Mode::RX;
            Format dataFmt = Format::F32;
            bool calibrate = true;
        };

        struct State {
            float frequency = 96.9e6;
            float manualGain = 0;
            bool enableAGC = true;
        };

        virtual ~Channel() = default;

        virtual Result GetFoundation(void*) = 0;
        virtual Result GetConfig(Config*) = 0;
        virtual Result GetState(State*) = 0;

        virtual Result Update(State, bool force=false) = 0;

        virtual Result ReadStream(void*, size_t, uint) = 0;
        virtual Result WriteStream(void*, size_t, uint) = 0;

        virtual Result SetupStream() = 0;
        virtual Result DestroyStream() = 0;
        virtual Result StartStream() = 0;
        virtual Result StopStream() = 0;
};

} // namespace Samurai

#endif
