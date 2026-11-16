#include <DomoticLink.h>

DomoticLink link;

void onFrame(const DomoticFrame &frame) {
    Serial.print("RX type=0x");
    Serial.print(frame.type, HEX);
    Serial.print(" len=");
    Serial.println(frame.length);

    link.send(0x02, frame.payload, frame.length);
}

void setup() {
    Serial.begin(115200);
    link.begin(Serial2, 115200);
    link.setCallback(onFrame);
    Serial.println("DomoticLink echo ready");
}

void loop() {
    link.update();
}
