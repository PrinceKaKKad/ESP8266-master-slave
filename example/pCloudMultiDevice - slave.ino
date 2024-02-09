#include "pCloudMultiDevice.h"

pCloudMultiDevice slaveDevice;  // Use a different instance name for the slave

void setup() {
    Serial.begin(115200);

    // Replace "slave1" with a unique identifier for each slave
    slaveDevice.begin("slave1", false);
}

void loop() {
    slaveDevice.handle();
    // Your slave-specific code here
}
