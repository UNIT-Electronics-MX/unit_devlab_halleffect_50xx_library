#include <Arduino.h>
#include <Wire.h>
#include <DevLab_HallEffect.h>

#if defined(ARDUINO_ARCH_RP2040)
  #if defined(PIN_WIRE1_SDA) && defined(PIN_WIRE1_SCL) && \
      PIN_WIRE1_SDA == 24U && PIN_WIRE1_SCL == 25U
    constexpr uint8_t I2C_SDA = 24U, I2C_SCL = 25U;
  #else
    constexpr uint8_t I2C_SDA = 12U, I2C_SCL = 13U;
  #endif
  constexpr uint32_t I2C_CLOCK_HZ = 100000U;
#elif defined(ARDUINO_ARCH_ESP32)
  constexpr uint8_t I2C_SDA = 6U, I2C_SCL = 7U;
  constexpr uint32_t I2C_CLOCK_HZ = 400000U;
#else
  #error "Use ESP32 or RP2040/RP2350"
#endif

DevLab_HallEffect hall(Wire, DevLab_HallEffect::DEFAULT_ADDRESS, I2C_CLOCK_HZ);

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!hall.beginRecovered(I2C_SDA, I2C_SCL)) {
    Serial.println("Hall module not found; check wiring, power and firmware");
    return;
  }
  hall.printInfo(Serial);
}

void loop() {
  if (!hall.isConnected()) return;

  uint16_t raw = 0;
  float milliTesla = 0.0f;
  if (!hall.readRaw(raw) || !hall.readMilliTesla(milliTesla)) {
    Serial.println("ERROR: Hall read failed");
    delay(500U);
    return;
  }
  Serial.print("raw=");
  Serial.print(raw);
  Serial.print(" B=");
  Serial.print(milliTesla, 1);
  Serial.println(" mT");
  delay(100U);
}
