#/usr/bin/env python3

import signal
import numpy as np
import samurai as s
import sounddevice as sd

from samurai import ASSERT_SUCCESS
from radio.analog import WBFM, Decimator

# Demodulator Settings
cuda = True
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
channelState.frequency = freq
channelState.enableAGC = True
ASSERT_SUCCESS(device.UpdateChannel(rx, channelState))

# Queue and Shared Memory Allocation
demod = WBFM(tau, mfs, afs, cuda=cuda)
dec = Decimator(sfs, mfs, cuda=cuda)

# Declare the memory buffer
if cuda:
    import cusignal as sig
    buff = sig.get_shared_mem(dsp_buff, dtype=np.complex64)
else:
    buff = np.zeros([dsp_buff], dtype=np.complex64)


# Demodulation Function
def process(outdata, f, t, s):
    ASSERT_SUCCESS(device.ReadStream(rx, buff, 100))
    outdata[:] = np.dstack(demod.run(dec.run(buff)))


# Start Collecting Data
with sd.OutputStream(blocksize=dsp_out, callback=process,
                     samplerate=afs, channels=2), device:
    input()
