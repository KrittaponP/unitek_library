#include "unitek_drive.h"
#include "platform/stm32_can_driver.h"

int main(void)
{
    UnitekDrive motor;
    unitek_drive_init(&motor, &stm32_can_driver, 0x181, 0x201);

    // send request
    unitek_request_param_async(&motor, UNITEK_PARAM_SPEED, 100);      // every 100ms
    unitek_request_param_async(&motor, UNITEK_PARAM_CURRENT, 100);    // every 100ms
    unitek_request_param_async(&motor, UNITEK_PARAM_IGBT_TEMP, 0x00); // one-shot

    unitek_set_speed(&motor, 3276);   // ~10% speed
    unitek_set_torque(&motor, 16384); // ~50% torque
    unitek_set_position(&motor, 100000);

    while (1)
    {
        uint8_t data[8];
        uint8_t len;
        uint32_t id;
        // receive data via HAL or your platform CAN
        if (can_receive(&id, data, &len))
        {
            unitek_handle_rx_frame(&motor, id, data, len);
        }
    }
}
