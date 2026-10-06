/**
 * @file readHall5013.ino
 * @brief Detecta la presencia de un imán con el módulo Hall Effect 5013 por I2C.
 *
 * Imprime por Serial "Magnet detected" / "No magnet" solo cuando cambia el estado.
 * Compatible con ESP32 y RP2040/RP2350.
 *
 * @author Jonathan Mejorado
 * @organization UNIT Electronics MX
 */

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
bool last_detected = false;
bool first_sample = true;

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

  bool detected = false;
  if (!hall.readDetected(detected)) {
    Serial.println("ERROR: Hall read failed");
    delay(500U);
    return;
  }
  if (first_sample || detected != last_detected) {
    Serial.println(detected ? "Magnet detected" : "No magnet");
    last_detected = detected;
    first_sample = false;
  }
  delay(25U);
}
