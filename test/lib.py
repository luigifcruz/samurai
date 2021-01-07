#!/usr/bin/env python

import samurai as s
import numpy as np

from samurai import ASSERT_SUCCESS

device = s.Device(s.DeviceId.Airspy)

deviceConfig = s.DeviceConfig()
deviceConfig.sampleRate = 10e6
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

with device:
    buffer = np.zeros(2048, dtype=np.complex64)
    ASSERT_SUCCESS(device.ReadStream(rx, buffer, 100))
    print(buffer)
    assert(np.any(buffer))
