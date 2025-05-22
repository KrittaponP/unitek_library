#include "unitek_temp.h"

typedef struct {
    uint16_t raw;
    int8_t degC;
} TempMap;

static const TempMap temp_table[] = {
    {28480, 125}, {28179, 120}, {27851, 115}, {27497, 110},
    {27114, 105}, {26702, 100}, {26261,  95}, {25792,  90},
    {25296,  85}, {24775,  80}, {24232,  75}, {23671,  70},
    {23097,  65}, {22515,  60}, {21933,  55}, {21357,  50},
    {20793,  45}, {20250,  40}, {19733,  35}, {19247,  30},
    {18797,  25}, {18387,  20}, {18017,  15}, {17688,  10},
    {17400,   5}, {17151,   0}, {16938,  -5}, {16757, -10},
    {16609, -15}, {16487, -20}, {16387, -25}, {16308, -30}
};

#define TEMP_TABLE_SIZE (sizeof(temp_table) / sizeof(temp_table[0]))

float unitek_temp_degC_from_raw(uint16_t raw) {
    // Clamp to range
    if (raw >= temp_table[0].raw)
        return (float)temp_table[0].degC;
    if (raw <= temp_table[TEMP_TABLE_SIZE - 1].raw)
        return (float)temp_table[TEMP_TABLE_SIZE - 1].degC;

    // Binary search
    int low = 0, high = TEMP_TABLE_SIZE - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (temp_table[mid].raw == raw)
            return (float)temp_table[mid].degC;

        if (temp_table[mid].raw > raw)
            low = mid + 1;
        else
            high = mid - 1;
    }

    // Interpolate between temp_table[low] and temp_table[low - 1]
    const TempMap* lower = &temp_table[low];
    const TempMap* upper = &temp_table[low - 1];

    float ratio = (float)(raw - lower->raw) / (upper->raw - lower->raw);
    return lower->degC + ratio * (upper->degC - lower->degC);
}
