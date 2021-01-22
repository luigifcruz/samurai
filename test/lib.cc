#include <samurai/samurai.hpp>

using namespace Samurai;

int main() {
    auto device = GetDeviceHandler(DeviceId::AirspyHF);

    Device::Config deviceConfig{};
    deviceConfig.sampleRate = 256e3;
    device->Enable(deviceConfig);

    ChannelId rx;
    Channel::Config channelConfig{};
    channelConfig.mode = Mode::RX;
    channelConfig.dataFmt = Format::F32;
    ASSERT_SUCCESS(device->EnableChannel(channelConfig, &rx));

    Channel::State channelState{};
    channelState.enableAGC = true;
    channelState.frequency = 96.9e6;
    ASSERT_SUCCESS(device->UpdateChannel(rx, channelState));

    {
        ASSERT_SUCCESS(device->StartStream());

        float buffer[2048];
        ASSERT_SUCCESS(device->ReadStream(rx, (float*)&buffer, 1024, 1000));
        printf("%lf\n", buffer[0]);

        ASSERT_SUCCESS(device->StopStream());
    }
}
