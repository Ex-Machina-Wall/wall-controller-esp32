#include <led_driver.h>

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Pixel_S allPixels[NUMPIXELS];

float RAMP_GAIN = 0.02;

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
    Serial.print("Pixel Targets Set...");
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
    for (uint8_t i = 0; i < NUMPIXELS; i++){
        iterPixel(&allPixels[i]);
    }
    for (uint8_t i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, allPixels[i].r, allPixels[i].g, allPixels[i].b);
    }
    pixels.show();
};

int getModification(int error, float ramp_rate){
    // if (error < 0){
    //     ramp_rate = ramp_rate * 2;
    //     if (ramp_rate > 1){
    //         ramp_rate = 1;
    //     }
    // }
    float modification = ((float(error)*ramp_rate));
    if ((modification > 0) & (modification < 1)){
      modification = 1;
    } else if (modification < 0 & modification > -1){
      modification = - 1;
    }
    return int(modification);
};

void iterPixel(Pixel_S *pixel){
    float ramp_rate = RAMP_GAIN;
    // if r > r_target then r_error will be positive, there for we need to subtract it
    // if r < r_target then r_error will be negative, therefor we still subtract it
    float r_error = pixel->r - pixel->r_target;
    float modification = getModification(r_error, ramp_rate);
    pixel->r = pixel->r - modification;
    
    float g_error = pixel->g - pixel->g_target;
    modification = getModification(g_error, ramp_rate);
    pixel->g = pixel->g - modification;

    float b_error = pixel->b - pixel->b_target;
    modification = getModification(b_error, ramp_rate);
    pixel->b = pixel->b - modification;
    
}