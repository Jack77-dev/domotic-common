#include "DomoticLink.h"

DomoticLink::DomoticLink()
    : _serial(nullptr), _callback(nullptr),
      _state(DL_WAIT_START), _rxIndex(0), _rxChecksum(0) {
    memset(&_rxFrame, 0, sizeof(_rxFrame));
}

void DomoticLink::begin(HardwareSerial &serial, uint32_t baud) {
    _serial = &serial;
    _serial->begin(baud);
    resetRx();
}

void DomoticLink::setCallback(DomoticLinkCallback cb) {
    _callback = cb;
}

bool DomoticLink::send(uint8_t type, const uint8_t *payload, uint8_t length) {
    if (_serial == nullptr) return false;
    if (length > DOMOTIC_LINK_MAX_PAYLOAD) return false;

    uint8_t checksum = DOMOTIC_LINK_START_BYTE ^ length ^ type;
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= payload[i];
    }

    _serial->write(DOMOTIC_LINK_START_BYTE);
    _serial->write(length);
    _serial->write(type);
    if (length > 0) {
        _serial->write(payload, length);
    }
    _serial->write(checksum);
    _serial->flush();
    return true;
}

bool DomoticLink::send(uint8_t type) {
    return send(type, nullptr, 0);
}

void DomoticLink::update() {
    if (_serial == nullptr) return;
    while (_serial->available() > 0) {
        processByte((uint8_t)_serial->read());
    }
}

void DomoticLink::resetRx() {
    _state = DL_WAIT_START;
    _rxIndex = 0;
    _rxChecksum = 0;
    _rxFrame.length = 0;
    _rxFrame.type = 0;
}

void DomoticLink::processByte(uint8_t b) {
    switch (_state) {
        case DL_WAIT_START:
            if (b == DOMOTIC_LINK_START_BYTE) {
                _rxChecksum = b;
                _state = DL_WAIT_LENGTH;
            }
            break;

        case DL_WAIT_LENGTH:
            if (b > DOMOTIC_LINK_MAX_PAYLOAD) {
                resetRx();
                break;
            }
            _rxFrame.length = b;
            _rxChecksum ^= b;
            _rxIndex = 0;
            _state = DL_WAIT_TYPE;
            break;

        case DL_WAIT_TYPE:
            _rxFrame.type = b;
            _rxChecksum ^= b;
            _state = (_rxFrame.length == 0) ? DL_WAIT_CHECKSUM : DL_WAIT_PAYLOAD;
            break;

        case DL_WAIT_PAYLOAD:
            _rxFrame.payload[_rxIndex++] = b;
            _rxChecksum ^= b;
            if (_rxIndex >= _rxFrame.length) {
                _state = DL_WAIT_CHECKSUM;
            }
            break;

        case DL_WAIT_CHECKSUM:
            if (b == _rxChecksum) {
                if (_callback != nullptr) {
                    _callback(_rxFrame);
                }
            }
            resetRx();
            break;

        default:
            resetRx();
            break;
    }
}
