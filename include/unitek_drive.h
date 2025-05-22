#ifndef UNITEK_DRIVE_H
#define UNITEK_DRIVE_H

#include <stdint.h>
#include "can_interface.h"
#include "unitek_regs.h"

typedef enum
{
    UNITEK_PARAM_SPEED = REGID_SPEED_ACTUAL_FILTERED,
    UNITEK_PARAM_CURRENT = REGID_CURRENT_ACTUAL_FILTERED,
    UNITEK_PARAM_DC_VOLTAGE = REGID_DC_VOLTAGE,
    UNITEK_PARAM_IGBT_TEMP = REGID_TEMP_IGBT,
    UNITEK_PARAM_POWER = REGID_POWER
} UnitekParam;

typedef void (*UnitekCallback)(UnitekDrive *drv, UnitekParam param, uint32_t value);

typedef struct
{
    CANInterface *can;
    uint32_t tx_id;
    uint32_t rx_id;
    UnitekCallback callback;
} UnitekDrive;

bool unitek_drive_init(UnitekDrive *drv, CANInterface *can, uint32_t tx_id, uint32_t rx_id, UnitekCallback cb);

bool unitek_request_param_async(UnitekDrive *drv, UnitekParam param, uint8_t interval_ms); // 0x00: one-shot, 0x01–0xFE: cyclic, 0xFF: stop
void unitek_handle_rx_frame(UnitekDrive *drv, uint32_t cob_id, const uint8_t *data, uint8_t len);

bool unitek_set_speed(UnitekDrive *drv, int16_t speed);
bool unitek_set_torque(UnitekDrive *drv, int16_t torque);

bool unitek_get_speed(int32_t *speed);
bool unitek_get_current(int32_t *current);
bool unitek_get_dc_voltage(int32_t *dc_voltage);
bool unitek_get_temperature(int32_t *temperature);
bool unitek_get_power(int32_t *power);

#endif // UNITEK_DRIVE_H
