#ifndef UNITEK_REGS_H
#define UNITEK_REGS_H

// ==================
// Control Commands
// ==================
#define REGID_SPEED_CMD 0x31  // Speed command value (int16_t)
#define REGID_TORQUE_CMD 0x90 // Torque command value (int16_t)
#define REGID_POS_DEST 0x6E   // Target position (int32_t)

// ==================
// Read Parameters
// ==================
#define REGID_SPEED_ACTUAL 0x30            // Actual speed (int16_t)
#define REGID_SPEED_ACTUAL_FILTERED 0xA8   // Actual speed filetered (int16_t)
#define REGID_CURRENT_ACTUAL 0x20          // Actual current (int16_t)
#define REGID_CURRENT_ACTUAL_FILTERED 0x5F // Actual current filtered (int16_t)
#define REGID_DC_VOLTAGE 0x08              // DC Voltage
#define REGID_TEMP_IGBT 0x4A               // IGBT Temperature
#define REGID_POWER 0xF6                   // IGBT Temperature
#define REGID_CURRENT_DEVICE 0xC6
#define REGID_CURRENT_200PC 0xD9
// ==================
// Read/Write Config
// ==================
#define REGID_SPEED_LIMIT 0x34     // Speed limit (int16)
#define REGID_SPEED_LIMIT_NEG 0x3E // Speed limit negative (int16)
#define REGID_SPEED_LIMIT_POS 0x3F // Speed limit positive (int16)

#endif // UNITEK_REGS_H
