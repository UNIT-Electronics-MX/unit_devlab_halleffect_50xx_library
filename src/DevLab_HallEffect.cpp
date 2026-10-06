#include "DevLab_HallEffect.h"

bool DevLab_HallEffect::begin() {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.begin();
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_HallEffect::begin(uint8_t sdaPin, uint8_t sclPin, uint32_t clock) {
    _verified = false;
    _clock = clock;
    _bus.setClock(_clock);
    _busReady = _bus.begin(sdaPin, sclPin);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_HallEffect::beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs, bool restart) {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.beginRecovered(sdaPin, sclPin, timeoutUs, restart);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

void DevLab_HallEffect::printInfo(Print &out) const {
    DevLabDDP::printDeviceInfo(out, _address, _info, _ddp.expectedDeviceId());
}

bool DevLab_HallEffect::readDetected(bool &detected) {
    return _verified && _ddp.readHall(_address, detected);
}

bool DevLab_HallEffect::readRaw(uint16_t &raw) {
    return _verified && _ddp.readHallRaw(_address, raw);
}

bool DevLab_HallEffect::readMilliTesla(float &milliTesla) {
    uint16_t raw;
    if (!readRaw(raw)) return false;
    /* VOUT = VCC/2 + B * 0.005 * VCC and the ADC reference is VCC. */
    milliTesla = ((float)raw / 4095.0f - 0.5f) / 0.005f;
    return true;
}
