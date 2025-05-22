#include "unitek_drive.h"

static uint32_t actualSpeed = 0;
static uint32_t actualCurrent = 0;
static uint32_t actualDcVoltage = 0;
static uint32_t actualTemperature = 0;
static uint32_t actualPower = 0;

// unitek data callback
void on_unitek_data(UnitekDrive *drv, UnitekParam param, uint32_t value)
{
    switch (param)
    {
    case UNITEK_PARAM_SPEED:
        actualSpeed = value;
        printf("Speed = %ld\n", value);
        break;
    case UNITEK_PARAM_CURRENT:
        actualCurrent = value;
        printf("Current = %ld\n", value);
        break;
    case UNITEK_PARAM_DC_VOLTAGE:
        actualDcVoltage = value;
        printf("DC Voltage = %ld\n", value);
        break;
    case UNITEK_PARAM_IGBT_TEMP:
        actualTemperature = value;
        printf("Temperature = %ld\n", value);
        break;
    case UNITEK_PARAM_POWER:
        actualPower = value;
        printf("Power = %ld\n", value);
        break;
    default:
        break;
    }
}

static bool send_reg16(UnitekDrive *drv, uint8_t regid, int16_t value)
{
    uint8_t data[3] = {regid, value & 0xFF, (value >> 8) & 0xFF};
    return drv->can->send(drv->rx_id, data, 3);
}

static bool send_reg32(UnitekDrive *drv, uint8_t regid, int32_t value)
{
    uint8_t data[5] = {
        regid,
        (uint8_t)(value & 0xFF),
        (uint8_t)((value >> 8) & 0xFF),
        (uint8_t)((value >> 16) & 0xFF),
        (uint8_t)((value >> 24) & 0xFF)};
    return drv->can->send(drv->rx_id, data, 5);
}

bool unitek_drive_init(UnitekDrive *drv, CANInterface *can, uint32_t tx_id, uint32_t rx_id, UnitekCallback cb)
{
    drv->can = can;
    drv->tx_id = tx_id;
    drv->rx_id = rx_id;
    if (cb)
    { // user define callback function
        drv->callback = cb;
    }
    else
    { // default define callback function
        drv->callback = on_unitek_data;
    }
    return true;
}

bool unitek_request_param_async(UnitekDrive *drv, UnitekParam param, uint8_t interval_ms)
{
    uint8_t data[3] = {0x3D, (uint8_t)param, interval_ms};
    return drv->can->send(drv->rx_id, data, 3);
}

void unitek_handle_rx_frame(UnitekDrive *drv, uint32_t cob_id, const uint8_t *data, uint8_t len)
{
    if (cob_id != drv->tx_id || len < 3)
        return;

    uint8_t regid = data[0];
    uint32_t value = 0;

    // 16-bit value (3 bytes: regid + data1 + data2 )
    if (len == 3)
    {
        value = data[1] | (data[2] << 8);
    }
    // 32-bit value (5 bytes: regid + data1–4)
    else if (len == 5)
    {
        value = (data[1]) |
                (data[2] << 8) |
                (data[3] << 16) |
                (data[4] << 24);
    }
    else
    {
        return; // unsupported format
    }

    if (drv->callback)
    {
        drv->callback(drv, (UnitekParam)regid, value);
    }
}

bool unitek_set_speed(UnitekDrive *drv, int16_t speed)
{
    return send_reg16(drv, REGID_SPEED_CMD, speed);
}

bool unitek_set_torque(UnitekDrive *drv, int16_t torque)
{
    return send_reg16(drv, REGID_TORQUE_CMD, torque);
}

bool unitek_get_speed(int32_t *speed)
{
    if (speed)
    {
        *speed = actualSpeed;
        return true;
    }
    return false;
}
bool unitek_get_current(int32_t *current)
{
    if (current)
    {
        *current = actualCurrent;
        return true;
    }
    return false;
}
bool unitek_get_dc_voltage(int32_t *dc_voltage)
{
    if (dc_voltage)
    {
        *dc_voltage = actualDcVoltage;
        return true;
    }
    return false;
}
bool unitek_get_temperature(int32_t *temperature)
{
    if (temperature)
    {
        *temperature = actualTemperature;
        return true;
    }
    return false;
}
bool unitek_get_power(int32_t *power)
{
    if (power)
    {
        *power = actualPower;
        return true;
    }
    return false;
}
