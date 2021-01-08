#include "samurai/base/channel.hpp"

namespace Samurai {

Result Channel::Update(State s, bool force) {
    force = force || !configured;

    auto res = update(s, state, force);
    ASSERT_RESULT(res);

    this->configured = true;
    this->state = s;

exception:
    return res;
}

size_t Channel::BufferOccupancy() {
    return cb->Occupancy();
}

Result Channel::WaitBufferOccupancy(size_t size) {
    return cb->WaitBufferOccupancy(size);
}

Result Channel::ReadStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    return readStream(buffer, size, timeout_ms);
}

Result Channel::WriteStream(void* buffer, size_t size, uint timeout_ms) {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    return writeStream(buffer, size, timeout_ms);
}

Result Channel::SetupStream() {
    if (stream.created || stream.running || !configured)
        return Result::ERROR_CHANNEL_NOT_READY;

    auto res = setupStream();
    ASSERT_RESULT(res);

    cb = std::make_unique<CircularBuffer<float>>(config.bufferSize);

    stream.created = true;

exception:
    return res;
}

Result Channel::DestroyStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    auto res = destroyStream();
    ASSERT_RESULT(res);

    cb.reset();

    stream.created = false;

exception:
    return res;
}

Result Channel::StartStream() {
    if (!stream.created || stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    auto res = startStream();
    ASSERT_RESULT(res);

    stream.running = true;

exception:
    return res;
}

Result Channel::StopStream() {
    if (!stream.created || !stream.running)
        return Result::ERROR_CHANNEL_NOT_READY;

    auto res = stopStream();
    ASSERT_RESULT(res);

    stream.running = false;

exception:
    return res;
}

Result Channel::GetConfig(Config* config) {
    *config = this->config;
    return Result::SUCCESS;
}

} // namespace Samurai
