#include <led_driver.h>

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Pixel_S allPixels[NUMPIXELS];
float last_pixel_set_time = millis();
float average_time_between_pixel_target_updates = 0;
float RAMP_GAIN = 0.02;

float time_spent_writing_leds = 0;
float time_spent_writing_leds_avg = 0;

void setupPixels(){
    pixels.begin();
};

void setPixelTargets(DataPacket &datapacket){
    uint8_t iter = 0;
    for (uint8_t i = 0; i < NUMPIXELS; i++){
        uint8_t r = datapacket.rgb_list[iter];
        uint8_t g = datapacket.rgb_list[iter+1];
        uint8_t b = datapacket.rgb_list[iter+2];
        iter+=3;
        setPixelTarget(&allPixels[i], r, g, b);
        // Serial.print("Setting pixel target: r"); 
        // Serial.print(r); Serial.print(" g"); Serial.print(g); Serial.print(" b"); Serial.println(b);
    }
    float time_now = millis();
    // using last_pixel_set_time and time_now to calculate the time it takes to set the pixels
    float duration = time_now - last_pixel_set_time;
    Serial.print(">ThisDuration:"); Serial.println(duration);
    last_pixel_set_time = time_now;
    average_time_between_pixel_target_updates = average_time_between_pixel_target_updates*0.9 + duration*0.1;
    Serial.print(">AvgDuration:"); Serial.println(average_time_between_pixel_target_updates);
    // Serial.print("Pixel Targets Set...");
};

void setRampGain(int new_gain){
    RAMP_GAIN = float(new_gain)/1000;
};

void setPixelTarget(Pixel_S *pixel, uint8_t r_new, uint8_t g_new, uint8_t b_new){
    pixel->r_target = r_new;
    pixel->g_target = g_new;
    pixel->b_target = b_new;
};

void updatePixelColors(){
    float time_now = millis();
    for (uint8_t i = 0; i < NUMPIXELS; i++){
        iterPixel(&allPixels[i]);
    }
    for (uint8_t i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, allPixels[i].r, allPixels[i].g, allPixels[i].b);
    }
    pixels.show();
    time_spent_writing_leds = millis() - time_now;
    time_spent_writing_leds_avg = time_spent_writing_leds_avg*0.9 + time_spent_writing_leds*0.1;
    Serial.print(">ThisWriteDuration:"); Serial.println(time_spent_writing_leds);
    Serial.print(">AvgWriteDuration:"); Serial.println(time_spent_writing_leds_avg);
};

void iterPixel(Pixel_S *pixel){
    float ramp_rate = RAMP_GAIN;
    // if r > r_target then r_error will be positive, there for we need to subtract it
    // if r < r_target then r_error will be negative, therefor we still subtract it
    float r_error = pixel->r - pixel->r_target;
    pixel->r = int(float(pixel->r) - ((float(r_error)*ramp_rate) + 0.5));
    
    float g_error = pixel->g - pixel->g_target;
    pixel->g = int(float(pixel->g) - ((float(g_error)*ramp_rate) + 0.5));

    float b_error = pixel->b - pixel->b_target;
    pixel->b = int(float(pixel->b) - ((float(b_error)*ramp_rate) + 0.5));
}