#ifndef SAMURAI_CHANNEL_H
#define SAMURAI_CHANNEL_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

#include "samurai/base/type.hpp"
#include "samurai/base/cbuffer.hpp"

namespace Samurai {

class Channel {
    public:
        struct Config {
            Mode mode = Mode::RX;
            Format dataFmt = Format::F32;
            bool calibrate = true;
            size_t bufferSize = 1024 * 1024 * 8;
        };

        struct State {
            float frequency = 0;
            float manualGain = 0;
            bool enableAGC = false;
        };

        virtual ~Channel() = default;

        virtual Result GetFoundation(void*) = 0;
        Result GetConfig(Config*);

        Result Update(State, bool force=false);

        size_t BufferOccupancy();
        size_t BufferCapacity();
        Result WaitBufferOccupancy(size_t);

        Result ReadStream(void*, size_t, uint);
        Result WriteStream(void*, size_t, uint);

        Result SetupStream();
        Result DestroyStream();
        Result StartStream();
        Result StopStream();

    protected:
        struct Stream {
            bool created = false;
            bool running = false;
        };

        State state;
        Config config;
        Stream stream;
        bool configured = false;

        std::unique_ptr<CircularBuffer<float>> cb{};

        virtual Result enable() = 0;
        virtual Result update(State, State, bool) = 0;
        virtual Result readStream(void*, size_t, uint) = 0;
        virtual Result writeStream(void*, size_t, uint) = 0;
        virtual Result setupStream() = 0;
        virtual Result destroyStream() = 0;
        virtual Result startStream() = 0;
        virtual Result stopStream() = 0;
};

} // namespace Samurai

#endif
