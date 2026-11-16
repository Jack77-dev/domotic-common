# DomoticLink

Bibliothèque Arduino partagée entre les deux ESP32-C6 du projet domotique :

- **ESP32-C6 #1** : gateway Zigbee
- **ESP32-C6 #2** : IHM web (WiFi)

Communication point-à-point via **Serial2** (UART filaire).

## Protocole de trame

| Champ | Taille | Description |
|-------|--------|-------------|
| START | 1 octet | `0xAA` |
| LENGTH | 1 octet | Taille du payload (0-64) |
| TYPE | 1 octet | Identifiant du message |
| PAYLOAD | 0-64 octets | Données |
| CHECKSUM | 1 octet | XOR de tous les octets précédents |

## Installation

Cloner ce dépôt dans le dossier `Arduino/libraries/` :

```bash
cd ~/Arduino/libraries
git clone https://github.com/Jack77-dev/domotic-common.git
```

Ou via le gestionnaire de bibliothèques Arduino (URL GitHub).

## Utilisation

```cpp
#include <DomoticLink.h>

DomoticLink link;

void onFrame(const DomoticFrame &frame) {
    // traitement de la trame reçue
}

void setup() {
    link.begin(Serial2, 115200);
    link.setCallback(onFrame);
}

void loop() {
    link.update();
    link.send(0x01, (uint8_t*)"ping", 4);
}
```

Voir `examples/BasicEcho` pour un exemple complet.
