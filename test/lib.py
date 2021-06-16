#!/usr/bin/env python

import samurai as s
import numpy as np

from samurai import CHECK

device = s.Device(s.DeviceId.Airspy)

deviceConfig = s.DeviceConfig()
deviceConfig.sampleRate = 10e6
CHECK(device.Enable(deviceConfig))

channelConfig = s.ChannelConfig()
channelConfig.mode = s.Mode.RX
channelConfig.dataFmt = s.Format.F32
rx, err = device.EnableChannel(channelConfig)
CHECK(err)

channelState = s.ChannelState()
channelState.frequency = 96.9e6
channelState.enableAGC = True
CHECK(device.UpdateChannel(rx, channelState))

with device:
    buffer = np.zeros(2048, dtype=np.complex64)
    CHECK(device.ReadStream(rx, buffer, 100))
    print(buffer)
    assert(np.any(buffer))
