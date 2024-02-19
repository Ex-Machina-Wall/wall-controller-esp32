// receiver.cpp
#include "receiver.h"
#include "secrets.h"

// This is for receiving the data
uint8_t postData[MAX_POST_DATA_SIZE];
size_t postDataLength = 0;
bool newData = false;
uint8_t aNumber = 1;
DataPacket receivedData;

// AsyncWebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/strip_data");

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from the Ex Machina Wall!");

  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
    
  } else if(type == WS_EVT_DATA){

    // if (postDataLength + len <= MAX_POST_DATA_SIZE) {
    if (len <= MAX_POST_DATA_SIZE) {
        if (len == 220){
          memcpy(&receivedData, data, len);
          Serial.print("Data Length: "); Serial.println(len);
          Serial.print("Ramp Rate: "); Serial.println(receivedData.ramp_rate);
          newData = true;
        }
    } else {
        // Handle the case where the data exceeds the buffer size
        // You can choose to send an error response or take other actions as needed
    }
    client->binary(&aNumber, 1);
  }
}

void setup_receiver() {
    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.begin();
}

bool checkNewData() {
  return newData;
}

DataPacket getNewData() {
  newData = false;
  return receivedData;
}







