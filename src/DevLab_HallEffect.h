#ifndef DEVLAB_HALL_EFFECT_H
#define DEVLAB_HALL_EFFECT_H

#pragma once

#include "DevLabDDP.h"
#include "DevLabDDPConsole.h"
#include "DevLab_I2C_Orchestrator.h"

class DevLab_HallEffect
{
public:
    /* Factory I2C address of the Hall-effect module (STARTUP_I2C_ADDRESS). */
    static constexpr uint8_t DEFAULT_ADDRESS = 0x29U;

    explicit DevLab_HallEffect(TwoWire &wire = Wire, uint8_t address = DEFAULT_ADDRESS, uint32_t clock = 400000UL)
    : _bus(wire, clock), _ddp(_bus, DevLabDDP::DEVICE_HALL), _address(address), _clock(clock) {}

    bool begin();
    bool begin(uint8_t sdaPin, uint8_t sclPin, uint32_t clock = 400000UL);
    bool beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs = 20000, bool restart = false);

    /* Returns false on an I2C error. `detected` is true while a magnet is
     * sensed by the module. */
    bool readDetected(bool &detected);

    /* Analog module (DRV5055). `raw` is the 12-bit ADC sample; 2048 is 0 mT.
     * readMilliTesla converts it for the DRV5055A3 (25 mV/mT at 5 V, i.e.
     * 0.5 % of VCC per mT, ratiometric); positive/negative follows the pole. */
    bool readRaw(uint16_t &raw);
    bool readMilliTesla(float &milliTesla);

    bool busReady() const { return _busReady; }
    bool isConnected() const { return _verified; }
    uint8_t address() const { return _address; }
    const DevLabDDP::DeviceInfo &deviceInfo() const { return _info; }
    void printInfo(Print &out = Serial) const;

    DevLabDDP::Master &protocol() { return _ddp; }
    DevLab_I2C_Orchestrator &bus() { return _bus; }

private:
    DevLab_I2C_Orchestrator _bus;
    DevLabDDP::Master _ddp;
    uint8_t _address;
    uint32_t _clock;
    bool _busReady = false;
    bool _verified = false;
    DevLabDDP::DeviceInfo _info;
};

#endif
