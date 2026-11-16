#include <DomoticLink.h>

// NB: ne pas appeler la variable "link" (conflit avec la fonction POSIX link() de unistd.h)
DomoticLink domoticLink;

void onFrame(const DomoticFrame &frame) {
    Serial.print("RX type=0x");
    Serial.print(frame.type, HEX);
    Serial.print(" len=");
    Serial.println(frame.length);

    domoticLink.send(0x02, frame.payload, frame.length);
}

void setup() {
    Serial.begin(115200);
    domoticLink.begin(Serial2, 115200);
    domoticLink.setCallback(onFrame);
    Serial.println("DomoticLink echo ready");
}

void loop() {
    domoticLink.update();
}
