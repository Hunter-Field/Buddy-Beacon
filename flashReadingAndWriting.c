#include "packets.h"
#include <stdbool.h>

#define MAX_BEACONS 10
BeaconInfo beaconsArray[MAX_BEACONS];
uint32_t startBeaconArrayFlashAddress = 0x080E0000;


void storeBeaconsInFlash(){
    HAL_FLASH_Unlock();
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    for (int i = 0; i < MAX_BEACONS; i++) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startBeaconArrayFlashAddress + (i * sizeof(BeaconInfo)), (uint32_t)&beaconsArray[i]);
    }
    HAL_FLASH_Lock();
}
void initializeBeaconArray(){
    for (int i = 0; i < MAX_BEACONS; i++) {
        beaconsArray[i].beaconID[0] = '\0';
        beaconsArray[i].beaconName[0] = '\0';
        beaconsArray[i].inRange = false;
        beaconsArray[i].accessThroughBeaconID[0] = '\0';
    }
}
void addBeaconToBeaconArray(BeaconInfo beacon){
    for(int j=0; j< MAX_BEACONS; j++ ){
        if (strcmp(beaconsArray[j].beaconID, beacon.beaconID) == 0) {
            // Update the existing beacon
            strcpy(beaconsArray[j].beaconName, beacon.beaconName);
            beaconsArray[j].inRange = beacon.inRange;
            strcpy(beaconsArray[j].accessThroughBeaconID, beacon.accessThroughBeaconID);
            break;
        } else if (strcmp(beaconsArray[j].beaconID,'\0') == 0 || beaconsArray[j].inRange == false) {
            // Add a new beacon
            strcpy(beaconsArray[j].beaconID, beacon.beaconID);
            strcpy(beaconsArray[j].beaconName, beacon.beaconName);
            beaconsArray[j].inRange = beacon.inRange;
            strcpy(beaconsArray[j].accessThroughBeaconID, beacon.accessThroughBeaconID);
            break;
        }
    }
    storeBeaconsInFlash();
}
void handlePacket7(BeaconPacket packet) {
    for (int i = 0; i < packet.data.beaconList.endingNum - packet.data.beaconList.startingNum + 1; i++) {
        BeaconInfo beacon;
        memcpy(beacon.beaconID, packet.data.beaconList.beacons[i].beaconID, 12);
        memcpy(beacon.beaconName, packet.data.beaconList.beacons[i].beaconName, 16);
        beacon.inRange = packet.data.beaconList.beacons[i].inRange;
        memcpy(beacon.accessThroughBeaconID, packet.tBeaconID, 12);
        addBeaconToBeaconArray(beacon);
    }
}
void handlePacket1(BeaconPacket packet) {
    BeaconInfo newBeacon;
    memcpy(newBeacon.beaconID, packet.tBeaconID, 12);
    memcpy(newBeacon.beaconName, packet.data.initBeacon.tBeaconName, 16);
    newBeacon.inRange = true; 
    newBeacon.accessThroughBeaconID[0] = '\0';
    addBeaconToBeaconArray(newBeacon);
}
void handlePacket2(BeaconPacket packet) {
    BeaconInfo newBeacon;
    memcpy(newBeacon.beaconID, packet.tBeaconID, 12);
    memcpy(newBeacon.beaconName, packet.data.replyNearby.tBeaconName, 16);
    newBeacon.inRange = true; 
    newBeacon.accessThroughBeaconID[0] = '\0';
    addBeaconToBeaconArray(newBeacon);
}