#include <samurai.h>

using namespace Samurai::LimeSDR;

void CHECK_ERR(int res) {
    if (res != 0) {
        std::cout << "Error..." << std::endl;
        exit(0);
    }
}

int main() {
    Device::Config deviceConfig{};
    deviceConfig.sampleRate = 10e6;
    Device device(deviceConfig);

    Channel::Config channelConfig{};
    channelConfig.enableAGC = true;
    channelConfig.frequency = 96.9e6;
    channelConfig.fdn.mode = Mode::RX;
    channelConfig.fdn.dataFmt = Format::F32;
    auto& ch = device.EnableChannel(channelConfig);

    {
        CHECK_ERR(device.StartStream());

        float buffer[2048];
        ch.ReadStream((float*)&buffer, 1024, 1000);
        printf("%lf\n", buffer[0]);

        CHECK_ERR(device.StopStream());
    }
}
