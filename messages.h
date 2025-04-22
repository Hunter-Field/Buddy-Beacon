#define MAX_MESSAGE_LEN 200
#include <stdint.h>
#include <stdbool.h>
typedef struct
{
    bool didIReceive;
    char message[MAX_MESSAGE_LEN];
} Message;

typedef struct {
    char BeaconID[12];
    Message messages[20];
    uint8_t startingMessageIndex;
    
} MessageHistoryElement;