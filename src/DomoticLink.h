#ifndef DOMOTIC_LINK_H
#define DOMOTIC_LINK_H

#include <Arduino.h>

#define DOMOTIC_LINK_START_BYTE 0xAA
#define DOMOTIC_LINK_MAX_PAYLOAD 64

enum DomoticLinkState : uint8_t {
    DL_WAIT_START = 0,
    DL_WAIT_LENGTH,
    DL_WAIT_TYPE,
    DL_WAIT_PAYLOAD,
    DL_WAIT_CHECKSUM
};

struct DomoticFrame {
    uint8_t type;
    uint8_t length;
    uint8_t payload[DOMOTIC_LINK_MAX_PAYLOAD];
};

typedef void (*DomoticLinkCallback)(const DomoticFrame &frame);

class DomoticLink {
public:
    DomoticLink();

    void begin(HardwareSerial &serial, uint32_t baud);
    void setCallback(DomoticLinkCallback cb);

    bool send(uint8_t type, const uint8_t *payload, uint8_t length);
    bool send(uint8_t type);

    void update();

private:
    HardwareSerial *_serial;
    DomoticLinkCallback _callback;

    DomoticLinkState _state;
    DomoticFrame _rxFrame;
    uint8_t _rxIndex;
    uint8_t _rxChecksum;

    void resetRx();
    void processByte(uint8_t b);
};

#endif
