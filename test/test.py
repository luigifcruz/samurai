import samurai as s
import numpy as np

device = s.Device(s.DeviceId.LimeSDR)

deviceConfig = s.DeviceConfig()
deviceConfig.sampleRate = 20e6
device.Enable(deviceConfig)

channelConfig = s.ChannelConfig()
channelConfig.mode = s.Mode.RX
channelConfig.dataFmt = s.Format.F32
rx, _ = device.EnableChannel(channelConfig)

channelState = s.ChannelState()
channelState.frequency = 96.9e6
channelState.enableAGC = True
device.UpdateChannel(rx, channelState)

with device:
    buffer = np.zeros(2048, dtype=np.complex64)
    device.ReadStream(rx, buffer, 100)
    print(buffer[-8:])
