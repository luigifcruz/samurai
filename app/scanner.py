import sys
import numpy as np
import samurai as s

import matplotlib.pyplot as plt
from scipy import signal as sg
from samurai import CHECK
from common import Mold

# Manual configuration
i_freq = int(70e6)
f_freq = int(500e6)

samplerate = int(10e6)
n_frames = int(50e3)
n_bframes = int(10e3)

# Make band plan
if (i_freq - f_freq) % samplerate != 0:
    raise ValueError("Frequency range needs to be divisable by samplerate.")

if n_frames % n_bframes != 0:
    raise ValueError("Number of frames need to be divisable by n_bframes.")

freqs = []
for z in range(i_freq, f_freq, samplerate):
    freqs.append(z + (samplerate / 2))

print(f"Frequencies: {freqs}")

# Configure SDR
device = s.Device(s.DeviceId.Airspy)

deviceConfig = s.DeviceConfig()
deviceConfig.sampleRate = samplerate
CHECK(device.Enable(deviceConfig))

channelConfig = s.ChannelConfig()
channelConfig.mode = s.Mode.RX
channelConfig.dataFmt = s.Format.F32
rx, err = device.EnableChannel(channelConfig)
CHECK(err)

channelState = s.ChannelState()
channelState.frequency = freqs[0]
channelState.enableAGC = True
CHECK(device.UpdateChannel(rx, channelState))

# Configure Mold
mold = Mold(samplerate, n_frames, len(freqs))

# Initialize
frame_pool = np.zeros((len(freqs), n_frames), dtype=np.complex64)

# Start data colletion
print("Sample collection starting.")

with device:
    for i, freq in enumerate(freqs):
        print(f"Tuning to {freq}. Using slot #{i}.")

        channelState.frequency = freq
        CHECK(device.UpdateChannel(rx, channelState))

        for offset in range(0, n_frames, n_bframes):
            start = offset
            end = offset + n_bframes
            CHECK(device.ReadStream(rx, frame_pool[i][start:end], 1000))

print("Sample collection finished.")

#for i, freq in enumerate(freqs):
#    plt.specgram(frame_pool[i], NFFT=2048, Fs=samplerate)
#    plt.ylim(-samplerate/2, samplerate/2)
#    plt.show()

sig = mold.inject(frame_pool)

samplerate *= len(freqs)
plt.specgram(sig, NFFT=2048, Fs=samplerate)
plt.ylim(-samplerate/2, samplerate/2)
plt.show()
