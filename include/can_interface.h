#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool (*send)(uint32_t id, const uint8_t *data, uint8_t len);
    bool (*receive)(uint32_t *id, uint8_t *data, uint8_t *len);
} CANInterface;

#endif // CAN_INTERFACE_H
