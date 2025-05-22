#include "example_stm32_can_driver.h"
#include "stm32fxxx_hal.h" // Adjust to match your chip

static bool can_send(uint32_t id, const uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef header = {
        .StdId = id,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = len};
    uint32_t mailbox;
    return (HAL_CAN_AddTxMessage(&hcan1, &header, (uint8_t *)data, &mailbox) == HAL_OK);
}

static bool can_receive(uint32_t *id, uint8_t *data, uint8_t *len)
{
    CAN_RxHeaderTypeDef header;
    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &header, data) != HAL_OK)
        return false;
    *id = header.StdId;
    *len = header.DLC;
    return true;
}

CANInterface stm32_can_driver = {
    .send = can_send,
    .receive = can_receive};
