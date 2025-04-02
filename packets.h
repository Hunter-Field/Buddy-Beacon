#ifndef BEACON_PACKETS_H
#define BEACON_PACKETS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_MESSAGE_LEN 210
#define MAX_BEACONS 20


// Packet 01 - Beacon Initialization
typedef struct {
    char tBeaconName[16];
} Packet01_InitBeacon;

// Packet 02 - Reply to Nearby Search
typedef struct {
    char tBeaconName[16];
} Packet02_ReplyNearby;

// Packet 03 - Requesting Beacon Coordinates
typedef struct {
} Packet03_RequestCoords;

// Packet 04 - Reply with Coordinates
typedef struct {
    float tLatitude;
    float tLongitude;
} Packet04_ReplyCoords;

// Packet 05 - Sending a Text Message
typedef struct {
    uint8_t messageLength;
    char message[MAX_MESSAGE_LEN];
    uint16_t randNum;
} Packet05_SendMessage;

// Packet 06 - Acknowledging Received Message
typedef struct {
    uint16_t randNum;
} Packet06_AckMessage;

// Beacon Struct (used in Packet 07)
typedef struct {
    char beaconID[12];
    char beaconName[16];
    bool inRange; 
    char accessThroughBeaconID[12];
} BeaconInfo;

// Packet 07 - Reply with Nearby Beacons
typedef struct {
    BeaconInfo beacons[MAX_BEACONS];
    uint8_t totalNum;
    uint8_t startingNum;
    uint8_t endingNum;
} Packet07_BeaconList;

typedef struct {
    uint8_t packetType;
    uint8_t ttl;
    char tBeaconPassThrough[12];
    char tBeaconID[12];
    char rBeaconID[12];
    union {
        Packet01_InitBeacon initBeacon;
        Packet02_ReplyNearby replyNearby;
        Packet03_RequestCoords requestCoords;
        Packet04_ReplyCoords replyCoords;
        Packet05_SendMessage sendMessage;
        Packet06_AckMessage ackMessage;
        Packet07_BeaconList beaconList;
    } data;
} BeaconPacket;

#endif
