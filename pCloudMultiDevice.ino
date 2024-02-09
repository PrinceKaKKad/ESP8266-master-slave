#include "pCloudMultiDevice.h"

// pCloudMultiDevice slaveDevice; // Use a different instance name for the slave
pCloudMultiDevice masterDevice;

void setup() {
    Serial.begin(115200);

    // Replace "slave1" with a unique identifier for each slave
    // slaveDevice.begin("slave1", false);
    // for master 
    masterDevice.begin("master", true);
}

void loop() {
    // slaveDevice.handle();
    masterDevice.handle();
}
