/*
  MqttLogger - offer print() interface like Serial but by publishing to a given mqtt topic. 
               Uses Serial as a fallback when no mqtt connection is available.

  Claus Denk
  https://androbi.com
*/

#ifndef MqttLogger_h
#define MqttLogger_h

#include <Arduino.h>
#include <Print.h>
#include "../../../networkDevices/NetworkDevice.h"

#define MQTT_MAX_PACKET_SIZE 1024

enum MqttLoggerMode {
    MqttAndSerialFallback = 0,
    SerialOnly = 1,
    MqttOnly = 2,
    MqttAndSerial = 3,
};

class MqttLogger : public Print
{
private:
    const char* topic;
    uint8_t* buffer;
    uint8_t* bufferEnd;
    uint16_t bufferCnt = 0, bufferSize = 0;
    NetworkDevice* client;
    MqttLoggerMode mode;
    void sendBuffer();

public:
    MqttLogger(MqttLoggerMode mode=MqttLoggerMode::MqttAndSerialFallback);
    MqttLogger(NetworkDevice* client, const char* topic, MqttLoggerMode mode=MqttLoggerMode::MqttAndSerialFallback);
    ~MqttLogger();

    void setClient(NetworkDevice* client);
    void setTopic(const char* topic);
    void setMode(MqttLoggerMode mode);
    void setRetained(boolean retained);
    
    virtual size_t write(uint8_t);
    using Print::write;
    
    uint16_t getBufferSize();
    boolean setBufferSize(uint16_t size);
};

#endif