# DevLab_HallEffect examples

| Example | Purpose |
|---|---|
| `hall/readHall5013` | DRV5013-type sensor: print "Magnet detected" / "No magnet" whenever the state changes. |
| `hall/readHall5032` | DRV5032 sensor (active-low digital output, samples at 5-80 Hz): same output, so changes may appear a few tens of ms late. |
| `hall/readHall5055` | DRV5055A3 analog sensor: print the raw ADC value and the field in mT. |
| `i2c/changeAddress` | Scan the bus and change the I2C address of a Hall-effect module (default `0x29`). |

All examples verify Device ID `0x0109` before sending any command.
