# DevLab_HallEffect

Arduino library for the I2C Hall-effect sensor module using the DevLab Device
Protocol (DDP) over I2C.

This library wraps the shared
[`DevLabDDP`](https://github.com/UNIT-Electronics-MX/unit_devlab_ddp_library)
master and the
[`DevLab_Interface`](https://github.com/UNIT-Electronics-MX/unit_devlab_interface_library)
`DevLab_I2C_Orchestrator` bus class into a single `DevLab_HallEffect` object.
Firmware: `unit_firmware_i2c_halleffect_module`.

Compatible with ESP32, RP2040/RP2350, STM32 and AVR.

# Features

- Device identification (Device ID `0x0109`) before any command
- `readDetected()` reports whether a magnet is present
- I2C address scan and reassignment (`0x08` to `0x77`), default address `0x29`
- Bus recovery (`beginRecovered`) for a slave left mid-transaction

# Quick Start Example

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <DevLab_HallEffect.h>

DevLab_HallEffect hall(Wire, DevLab_HallEffect::DEFAULT_ADDRESS, 400000);

void setup() {
  Serial.begin(115200);
  // ESP32: 6/7. RP2040/RP2350: 24/25 (Pulsar) or 12/13.
  if (!hall.beginRecovered(6, 7)) {
    Serial.println("Hall module not found");
  }
}

void loop() {
  bool detected;
  if (hall.readDetected(detected)) {
    Serial.println(detected ? "Magnet detected" : "No magnet");
  }
  delay(100);
}
```

# API

| Method | Description |
|---|---|
| `DevLab_HallEffect(wire, address, clock)` | Create the object (default `Wire`, `0x29`, 400 kHz). |
| `begin()` / `begin(sda, scl, clock)` | Start the bus and verify the device. |
| `beginRecovered(sda, scl, timeoutUs, restart)` | Same, clearing a stuck bus first. |
| `readDetected(detected)` | Digital sensors (5013, 5032): `true` while a magnet is detected. |
| `readRaw(raw)` | Analog sensor (DRV5055): 12-bit sample, 2048 = 0 mT. |
| `readMilliTesla(mT)` | DRV5055A3 field in mT (signed by pole). |
| `isConnected()` / `busReady()` | Result of the last `begin`. |
| `deviceInfo()` / `printInfo(out)` | Identity reported by the module. |
| `protocol()` / `bus()` | Access the underlying DDP master and I2C bus. |

All calls return `false` on an I2C error or if `begin` has not verified the
device.

# License

MIT, see [LICENSE](LICENSE).
