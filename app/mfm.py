#/usr/bin/env python3

import signal
import numpy as np
import samurai as s
import sounddevice as sd

from samurai import ASSERT_SUCCESS
from radio.analog import MFM, WBFM, Decimator

# Demodulator Settings
freq = 96.9e6
tau = 75e-6
sfs = int(2.5e6)
mfs = int(240e3)
afs = int(48e3)

dsp_buff = 1024 * 128
dec_out = int(np.ceil(dsp_buff/(sfs/mfs)))
dsp_out = int(np.ceil(dec_out/(mfs/afs)))

# Radio Configuration
device = s.Device(s.DeviceId.Airspy)

deviceConfig = s.DeviceConfig()
deviceConfig.sampleRate = sfs
ASSERT_SUCCESS(device.Enable(deviceConfig))

channelConfig = s.ChannelConfig()
channelConfig.mode = s.Mode.RX
channelConfig.dataFmt = s.Format.F32
rx, err = device.EnableChannel(channelConfig)
ASSERT_SUCCESS(err)

channelState = s.ChannelState()
channelState.frequency = 96.9e6
channelState.enableAGC = True
ASSERT_SUCCESS(device.UpdateChannel(rx, channelState))

# Queue and Shared Memory Allocation
demod = MFM(tau, mfs, afs)
dec = Decimator(sfs, mfs)

# Declare the memory buffer
buff = np.zeros(dsp_buff, dtype=np.complex64)

# Demodulation Function
def process(outdata, f, t, s):
    device.ReadStream(rx, buff, 100)
    b = demod.run(dec.run(buff))
    outdata[:] = b.astype(np.float32).tobytes()

# Start Data Collection
device.StartStream()

# Wait Buffer to Fill
device.WaitBufferOccupancy(rx, dsp_buff * 8)

# Start Collecting Data
with sd.RawOutputStream(blocksize=dsp_out, dtype=np.float32, callback=process,
                     samplerate=afs, channels=1):
    input()

device.StopStream()
