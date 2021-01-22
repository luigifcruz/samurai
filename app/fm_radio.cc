#include <vector>
#include <complex>
#include <iostream>
#include <fstream>

#include <samurai/samurai.hpp>

extern "C" {
#include <liquid/liquid.h>
}

using namespace Samurai;

bool keepgoing = true;

int main() {
    //////// USER CONFIG
    float freq = 96.9e6;
    float samplerate = 2.5e6;
    //
    float demod_fs = 240e3;
    float output_fs = 48e3;
    size_t buffer_size = 1024 * 1024;
    ////////

    auto device = Airspy::Device();

    Device::Config deviceConfig{};
    deviceConfig.sampleRate = samplerate;
    device.Enable(deviceConfig);

    ChannelId rx;
    Channel::Config channelConfig{};
    channelConfig.mode = Mode::RX;
    channelConfig.dataFmt = Format::F32;
    ASSERT_SUCCESS(device.EnableChannel(channelConfig, &rx));

    Channel::State channelState{};
    channelState.enableAGC = true;
    channelState.frequency = freq;
    ASSERT_SUCCESS(device.UpdateChannel(rx, channelState));

    // Copy I/Q samples from SDR.
    const size_t a_len = buffer_size;
    auto a_buf = (std::complex<float>*)malloc(sizeof(std::complex<float>) * a_len);

    // Downsample to intermediate samplerate.
    float brf = demod_fs / samplerate;
    msresamp_crcf b_resamp = msresamp_crcf_create(brf, 60.0);

    const size_t b_len = ceilf(a_len * brf);
    auto b_buf = (std::complex<float>*)malloc(sizeof(std::complex<float>) * b_len);

    // Apply automatic gain control.
    agc_crcf agc = agc_crcf_create();
    agc_crcf_set_bandwidth(agc, 1e-3f);

    // Apply frequency demodulation.
    freqdem dem = freqdem_create(85e3 / demod_fs);

    const size_t c_len = b_len;
    auto c_buf = (float*)malloc(sizeof(float) * c_len);

    // Downsample real into output.
    float orf = output_fs / demod_fs;
    msresamp_rrrf o_resamp = msresamp_rrrf_create(orf, 60.0);

    const size_t d_len = ceilf(c_len * orf);
    auto d_buf = (float*)malloc(sizeof(float) * d_len);

    {
        ASSERT_SUCCESS(device.StartStream());

        while (keepgoing) {
            unsigned int len = a_len;

            ASSERT_SUCCESS(device.ReadStream(rx, a_buf, len, 1000));

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

            msresamp_crcf_execute(b_resamp, a_buf, len, b_buf, &len);

            for (size_t i = 0; i < len; i++)
                agc_crcf_execute(agc, b_buf[i], &b_buf[i]);

            freqdem_demodulate_block(dem, b_buf, len, c_buf);

            msresamp_rrrf_execute(o_resamp, c_buf, len, d_buf, &len);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            //fwrite(d_buf, sizeof(float), len, stdout);

            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
            std::cout << "Max allowed = " << (int)((len/output_fs)*1000) << "[ms]" << std::endl;
        }

        ASSERT_SUCCESS(device.StopStream());
    }

    msresamp_crcf_destroy(b_resamp);
    msresamp_rrrf_destroy(o_resamp);
    agc_crcf_destroy(agc);
    freqdem_destroy(dem);

    return 0;
}

