// receiver.h

// Include guards to prevent multiple inclusion
#ifndef RECEIVER_H
#define RECEIVER_H

#define MAX_POST_DATA_SIZE 1024 // You can adjust the size based on your requirements

#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// #include "WiFi.h"
// #include "ESPAsyncWebServer.h"

// Define your struct
struct DataPacket {
    uint8_t ramp_rate;
    uint8_t rgb_list[219];
};

void setup_receiver();
bool checkNewData();
DataPacket getNewData();

#endif // RECEIVER_H