#include <stdio.h>
#include <stdint.h>
#include "packets.h"
#include <string.h>
#define MAX_LEN 240

#define PACKET_MAX_SIZE sizeof(BeaconPacket)
#define MAX_SERIALIZED_SIZE 240
#define MAX_MESSAGE_LEN 200
size_t serializeBeaconPacket(const BeaconPacket *packet, uint8_t *buffer) {
    if (!packet || !buffer) return 0;
    size_t offset = 0;
    memcpy(buffer + offset, &packet->packetType, sizeof(packet->packetType));
    offset += sizeof(packet->packetType);
    memcpy(buffer + offset, &packet->ttl, sizeof(packet->ttl));
    offset += sizeof(packet->ttl);
    memcpy(buffer + offset, packet->tBeaconPassThrough, 12);
    offset += 12;
    memcpy(buffer + offset, packet->tBeaconID, 12);
    offset += 12;
    memcpy(buffer + offset, packet->rBeaconID, 12);
    offset += 12;
    switch (packet->packetType) {
        case 1:
            memcpy(buffer + offset, packet->data.initBeacon.tBeaconName, 16);
            offset += 16;
            break;
        case 2:
            memcpy(buffer + offset, packet->data.replyNearby.tBeaconName, 16);
            offset += 16;
            break;
        case 4:
            memcpy(buffer + offset, &packet->data.replyCoords.tLatitude, sizeof(float));
            offset += sizeof(float);
            memcpy(buffer + offset, &packet->data.replyCoords.tLongitude, sizeof(float));
            offset += sizeof(float);
            break;
        case 5:
            memcpy(buffer + offset, &packet->data.sendMessage.messageLength, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, packet->data.sendMessage.message, packet->data.sendMessage.messageLength);
            offset += packet->data.sendMessage.messageLength;
            memcpy(buffer + offset, &packet->data.sendMessage.randNum, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            break;
        case 6:
            memcpy(buffer + offset, &packet->data.ackMessage.randNum, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            break;
        case 7:
            memcpy(buffer + offset, &packet->data.beaconList.totalNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, &packet->data.beaconList.startingNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, &packet->data.beaconList.endingNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            uint8_t numBeingSent = packet->data.beaconList.endingNum - packet->data.beaconList.startingNum + 1;
            for (int i = 0; i < numBeingSent; i++) {
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].beaconID, 12);
                offset += 12;
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].beaconName, 16);
                offset += 16;
                memcpy(buffer + offset, &packet->data.beaconList.beacons[i].inRange, sizeof(bool));
                offset += sizeof(bool);
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].accessThroughBeaconID, 12);
                offset += 12;
            }
            break;
        default:
            return -1;
    }
    return offset;
}

void deserializeBeaconPacket(const uint8_t *buffer, BeaconPacket *packet) {
    if (!buffer || !packet) return;
    size_t offset = 0;
    memcpy(&packet->packetType, buffer + offset, sizeof(packet->packetType));
    offset += sizeof(packet->packetType);
    memcpy(&packet->ttl, buffer + offset, sizeof(packet->ttl));
    offset += sizeof(packet->ttl);
    memcpy(packet->tBeaconPassThrough, buffer + offset, 12);
    offset += 12;
    memcpy(packet->tBeaconID, buffer + offset, 12);
    offset += 12;
    memcpy(packet->rBeaconID, buffer + offset, 12);
    offset += 12;
    switch (packet->packetType) {
        case 1: 
            memcpy(packet->data.initBeacon.tBeaconName, buffer + offset, 16);
            break;
        case 2: 
            memcpy(packet->data.replyNearby.tBeaconName, buffer + offset, 16);
            break;
        case 4:
            memcpy(&packet->data.replyCoords.tLatitude, buffer + offset, sizeof(float));
            offset += sizeof(float);
            memcpy(&packet->data.replyCoords.tLongitude, buffer + offset, sizeof(float));
            break;
        case 5:
            memcpy(&packet->data.sendMessage.messageLength, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(packet->data.sendMessage.message, buffer + offset, packet->data.sendMessage.messageLength);
            offset += packet->data.sendMessage.messageLength;
            memcpy(&packet->data.sendMessage.randNum, buffer + offset, sizeof(uint16_t));
            break;
        case 6:
            memcpy(&packet->data.ackMessage.randNum, buffer + offset, sizeof(uint16_t));
            break;
        case 7:
            memcpy(&packet->data.beaconList.totalNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(&packet->data.beaconList.startingNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(&packet->data.beaconList.endingNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            uint8_t numBeingSent = packet->data.beaconList.endingNum - packet->data.beaconList.startingNum + 1;
            for (int i = 0; i < numBeingSent; i++) {
                memcpy(packet->data.beaconList.beacons[i].beaconID, buffer + offset, 12);
                offset += 12;
                memcpy(packet->data.beaconList.beacons[i].beaconName, buffer + offset, 16);
                offset += 16;
                memcpy(&packet->data.beaconList.beacons[i].inRange, buffer + offset, sizeof(bool));
                offset += sizeof(bool);
                memcpy(packet->data.beaconList.beacons[i].accessThroughBeaconID, buffer + offset, 12);
                offset += 12;
            }
            break;
        default:

            return;
    }
}

void printSerializedData(const uint8_t *buffer, size_t size) {
    printf("Serialized Packet Data (%zu bytes):\n", size);
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int validate_packet(const char *str, size_t len) {
    if (len == 0 || len > MAX_LEN) {
        return 0;
    }

    uint8_t firstByte = (uint8_t)str[0];

    return (firstByte <= 7) ? 1 : 0;
}

int main() {
    BeaconPacket packet;
    uint8_t buffer[MAX_SERIALIZED_SIZE];
    
    packet.packetType = 1;
    packet.ttl = 0;
    snprintf(packet.tBeaconPassThrough, 12, "");
    snprintf(packet.tBeaconID, 12, "T12345678901");
    snprintf(packet.data.initBeacon.tBeaconName, 16, "Beacon A");

    size_t serializedSize = serializeBeaconPacket(&packet, buffer);
    printf("Serialized size: %zu bytes\n", serializedSize);
    printSerializedData(buffer, serializedSize);

    BeaconPacket receivedPacket;
    deserializeBeaconPacket(buffer, &receivedPacket);

    printf("Deserialized Packet Type: %d\n", receivedPacket.packetType);
    printf("Deserialized TTL: %d\n", receivedPacket.ttl);
    printf("Deserialized Beacon ID: %s\n", receivedPacket.tBeaconID);
    printf("Deserialized Beacon Name: %s\n", receivedPacket.data.initBeacon.tBeaconName);


    return 0;
}
