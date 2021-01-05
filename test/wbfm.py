#/usr/bin/env python3

import samurai as s
from samurai import ASSERT_SUCCESS

import signal
import queue
import numpy as np
from radio.analog import WBFM, Decimator
import sounddevice as sd

# Demodulator Settings
freq = 96.9e6
tau = 75e-6
sfs = int(2.5e6)
mfs = int(240e3)
afs = int(48e3)

dsp_buff = 8192 * 33
dec_out = int(np.ceil(dsp_buff/(sfs/mfs)))
dsp_out = int(np.ceil(dec_out/(mfs/afs)))

# SoapySDR Configuration
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
que = queue.Queue()
demod = WBFM(tau, mfs, afs, cuda=True)
dec = Decimator(sfs, mfs, cuda=True)
buff = np.zeros([dsp_buff], dtype=np.complex64)
audio_file = open("FM_{}.if32".format(int(freq)), "bw")


# Demodulation Function
def process(outdata, f, t, s):
    outdata[:] = np.dstack(demod.run(dec.run(que.get())))
    LPR = outdata[:].astype(np.float32)
    audio_file.write(LPR.tobytes())


# Start Collecting Data
with sd.OutputStream(blocksize=dsp_out, callback=process,
                     samplerate=afs, channels=2), device:
    while True:
        ASSERT_SUCCESS(device.ReadStream(rx, buff, 1000))
        que.put(buff.copy())
