#include "pCloudMultiDevice.h"

pCloudMultiDevice masterDevice;

void setup() {
    Serial.begin(115200);

    masterDevice.begin("master", true);
}

void loop() {
    masterDevice.handle();
    // Your master-specific code here
}
