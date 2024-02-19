#include <Arduino.h>
#include <receiver.h>
#include <led_driver.h>

struct BrightnessControl {
  uint8_t current_value = 0; 
};

void writePixelDataToStrip(void * parameter);

void setup() {
  // Start Serial communication
  // Serial.begin(115200);
  Serial.begin(921600);

  setup_receiver();
  setupPixels();

  // xTaskCreate(
  //   writePixelDataToStrip,    // Function that should be called
  //   "writePixelDataToStrip",   // Name of the task (for debugging)
  //   1000,            // Stack size (bytes)
  //   NULL,            // Parameter to pass
  //   2,               // Task priority
  //   NULL             // Task handle
  // );

}

void writePixelDataToStrip(void * parameter){
  while (true){
    Serial.println("Hello from the other side");
    updatePixelColors();
    // Delay 17 ms which is approximately 60 fps
    delay(17);
  }
  
}

void loop() {
  // bool newData = checkNewData();
  // if (newData) {
  //   DataPacket dataPacket = getNewData();
  //   setRampGain(dataPacket.ramp_rate);
  //   setPixelTargets(dataPacket);
  // }
  // // Delay 17 ms which is approximately 60 fps
  // delay(17);
}