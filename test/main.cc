#include <samurai.h>

using namespace Samurai;

void CHECK_ERR(int res) {
    if (res != 0) {
        std::cout << "Error..." << std::endl;
        exit(0);
    }
}

int main() {
    LimeSDR::Device device;
    device.SetSamplerate(10e6);

    auto& ch = device.EnableChannel(LimeSDR::MODE_RX);
    ch.SetFrequency(96.9e6);

    CHECK_ERR(device.StartStreams());

    float buffer[2048];
    ch.ReadStream((float*)&buffer, 1024, 1000);

    CHECK_ERR(device.StopStreams());
}
