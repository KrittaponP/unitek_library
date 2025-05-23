# Unitek CAN Motor Drive Library (C)

A portable C library to interface with Unitek motor drives via CAN bus.  
Designed for embedded systems with modular support for multiple MCUs (e.g., STM32, ESP32), this library enables command/control and status monitoring via Unitek's CAN protocol.

---

## 🚀 Features

- Supports speed and torque commands
- Periodic or one-shot status requests (speed, current, voltage, status word, etc.)
- Decodes CAN responses with optional callback hooks
- Pluggable CAN driver layer (hardware abstraction)

---

## 🛠️ Integration Example

```c
#include "unitek_drive.h"
#include "unitek_temp.h"
#include "platform/stm32_can_driver.h"

UnitekDrive motor;

void on_data(UnitekDrive* drv, UnitekParam param, uint32_t value) {
    if (param == UNITEK_PARAM_SPEED) {
        printf("Speed: %d rpm\n", (int16_t)value);
    }
}

int main(void) {
    unitek_drive_init(&motor, &stm32_can_driver, 0x181, 0x201, on_data);
    unitek_set_speed(&motor, 2000);
    unitek_request_param_async(&motor, UNITEK_PARAM_SPEED, 100);  // 100ms cycle
    while (1);
}
```

## 🧱 Architecture

1. CANInterface

Abstracts CAN driver functions with send() and receive() via function pointers.

```c
typedef struct {
    bool (_send)(uint32_t id, const uint8_t_ data, uint8_t len);
    bool (_receive)(uint32_t_ id, uint8_t* data, uint8_t* len);
} CANInterface;
```

2. UnitekDrive

Encapsulates control logic and response parsing.

- `unitek_set_speed()`

- `unitek_set_torque()`

- `unitek_request_param_async()`

- `unitek_handle_rx_frame()`

3. Platform Driver

Implements CANInterface for specific hardware (e.g. STM32 HAL, ESP32 TWAI, Linux SocketCAN).

🌡️ Temperature Conversion
Converts IGBT temperature raw value from REGID 0x4A to float °C with linear interpolation:

```c
uint16_t raw;
float temp = unitek_temp_degC_from_raw(raw); // e.g. 112.3 °C
```

## 📚 Supported REGIDs

| Function         | REGID |
| ---------------- | ----- |
| Speed Command    | 0x31  |
| Torque Command   | 0x90  |
| Actual Speed     | 0xA8  |
| Actual Current   | 0x5F  |
| DC Bus Voltage   | 0x08  |
| IGBT Temperature | 0x4A  |

(See unitek_regs.h for all definitions)

## ⚙️ Platform Integration

This library is **platform-independent**. It provides only **protocol-level logic** for Unitek motor drives over CAN.

### 🔌 User Responsibility

To integrate with your hardware, **you must implement the `CANInterface`**:

```c
typedef struct {
    bool (*send)(uint32_t id, const uint8_t* data, uint8_t len);
    bool (*receive)(uint32_t* id, uint8_t* data, uint8_t* len);
} CANInterface;
```

## 📌 Notes

- Use unitek_handle_rx_frame() in your CAN RX ISR or polling loop to parse periodic messages.

- For cyclic requests, use unitek_request_param_async() with 0x01–0xFE for interval in ms.

- To stop a cyclic request, call with 0xFF.

## 📥 TODO / Extensions

- Add fixed-point temperature conversion for float-less MCUs

- Add support for command acknowledgment or error codes

- Add position capture, brake control, and fault decoding
