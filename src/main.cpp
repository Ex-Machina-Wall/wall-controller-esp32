#include <Arduino.h>
#include <receiver.h>
#include <led_driver.h>

struct BrightnessControl {
  uint8_t current_value = 0; 
};

void setup() {
  // Start Serial communication
  // Serial.begin(115200);

  setup_receiver();
  setupPixels();
}

void loop() {
  bool newData = checkNewData();
  if (newData) {
    DataPacket dataPacket = getNewData();
    setRampGain(dataPacket.ramp_rate);
    setPixelTargets(dataPacket);
  }
  updatePixelColors();
}