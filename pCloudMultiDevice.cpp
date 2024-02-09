#include "pCloudMultiDevice.h"

#define EEPROM_SIZE 512 // Size of EEPROM memory in bytes

pCloudMultiDevice::pCloudMultiDevice() : server(80) {
    connectedToMaster = false;
}

void pCloudMultiDevice::begin(String deviceId, bool isMaster) {
    this->deviceId = deviceId;
    this->isMaster = isMaster;

    Serial.begin(115200); // Initialize serial communication

    Serial.println("Initializing device...");
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("master", "password");

    if (isMaster) {
        Serial.println("Starting as master...");
        WiFi.softAP(deviceId.c_str(), "password");
        server.begin();
        Serial.println("Master initialized.");
    } else {
        Serial.println("Starting as slave...");
        connectToMaster();
        sendDeviceInfo();
        loadDeviceInfo(); // Load device info when the slave initializes
        Serial.println("Slave initialized.");
    }
}

void pCloudMultiDevice::handle() {
    if (isMaster) {
        // Master-related tasks
        WiFiClient client = server.available();
        if (client) {
            Serial.println("Client connected.");
            String request = client.readStringUntil('\r');
            if (request.startsWith("GET /")) {
                serveWebpage(client);
            }
            client.flush();
            client.stop();
            Serial.println("Client disconnected.");
        }
    } else {
        // Slave-related tasks
        if (connectedToMaster && client.connected()) {
            // Implement code to send data to the master
        }
    }
}

void pCloudMultiDevice::serveWebpage(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html><head><title>Slave Details</title></head><body>");
    client.println("<h1>Slave Details</h1>");
    client.println("<p>This is the webpage served by the master node.</p>");
    client.println("</body></html>");
}

String pCloudMultiDevice::getSlaveDetails() {
    // Read device info from EEPROM and return
    String loadedData;
    char c;
    int i = 0;
    do {
        c = EEPROM.read(i);
        if (c != '\0') {
            loadedData += c;
        }
        i++;
    } while (c != '\0');

    Serial.println("Loaded device info from EEPROM: " + loadedData);
    return loadedData;
}

void pCloudMultiDevice::connectToMaster() {
    Serial.println("Connecting to WiFi...");
    while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected to WiFi");

    Serial.println("Connecting to Master...");
    client.connect("192.168.4.1", 80); // Replace with the IP address of the master
    if (client.connected()) {
        Serial.println("Connected to Master");
        connectedToMaster = true;
    } else {
        Serial.println("Connection to Master failed");
    }
}

void pCloudMultiDevice::sendDeviceInfo() {
    if (connectedToMaster && client.connected()) {
        Serial.println("Sending device info to master...");
        client.print("GET /");
        client.println(deviceId);
        client.println(" HTTP/1.1");
        client.println("Host: 192.168.4.1"); // Replace with the IP address of the master
        client.println("Connection: close");
        client.println();
        delay(1000); // Wait for the response
        receiveDeviceInfo();
        Serial.println("Device info sent.");
    }
}

void pCloudMultiDevice::receiveDeviceInfo() {
    String response = client.readStringUntil('\r');
    Serial.println("Received device info: " + response);
    
    // Parse the received device info
    int delimiterIndex = response.indexOf(',');
    if (delimiterIndex != -1) {
        String deviceId = response.substring(0, delimiterIndex);
        response.remove(0, delimiterIndex + 1);

        delimiterIndex = response.indexOf(',');
        if (delimiterIndex != -1) {
            String macAddress = response.substring(0, delimiterIndex);
            response.remove(0, delimiterIndex + 1);

            delimiterIndex = response.indexOf(',');
            if (delimiterIndex != -1) {
                String name = response.substring(0, delimiterIndex);
                response.remove(0, delimiterIndex + 1);

                // Assuming the next part is boardType and the last part is timestamp
                String boardType = response;
                
                // Save the device info
                saveDeviceInfo(deviceId, macAddress, name, boardType);
            }
        }
    }
}

void pCloudMultiDevice::saveDeviceInfo(String deviceId, String macAddress, String name, String boardType) {
    // Construct the device info string
    String deviceInfo = deviceId + "," + macAddress + "," + name + "," + boardType;

    // Convert String to char array
    char deviceInfoArray[deviceInfo.length() + 1];
    deviceInfo.toCharArray(deviceInfoArray, deviceInfo.length() + 1);

    // Write the device info to EEPROM
    for (int i = 0; i < deviceInfo.length(); ++i) {
        EEPROM.write(i, deviceInfoArray[i]);
    }

    // Write null terminator to mark end of string
    EEPROM.write(deviceInfo.length(), '\0');

    // Commit changes to EEPROM
    EEPROM.commit();

    Serial.println("Device info saved to EEPROM.");
}

void pCloudMultiDevice::loadDeviceInfo() {
    // Read device info from EEPROM
    String loadedData;
    char c;
    int i = 0;
    do {
        c = EEPROM.read(i);
        if (c != '\0') {
            loadedData += c;
        }
        i++;
    } while (c != '\0');

    Serial.println("Loaded device info from EEPROM: " + loadedData);
    // Process loaded data as needed
}
