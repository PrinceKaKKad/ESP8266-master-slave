#ifndef PCLOUDMULTIDEVICE_H
#define PCLOUDMULTIDEVICE_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

#define MAX_DEVICES 10

class pCloudMultiDevice {
public:
    pCloudMultiDevice();

    void begin(String deviceId, bool isMaster);
    void handle();

private:
    bool isMaster;
    String deviceId;
    WiFiServer server;
    WiFiClient client;
    ESP8266WiFiMulti wifiMulti;
    bool connectedToMaster;

    void connectToMaster();
    void sendDeviceInfo();
    void receiveDeviceInfo();
    void saveDeviceInfo(String deviceId, String macAddress, String name, String boardType);
    void loadDeviceInfo();
    String getSlaveDetails();
    void serveWebpage(WiFiClient& client);
};

#endif // PCLOUDMULTIDEVICE_H
