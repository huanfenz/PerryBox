#ifndef CRC_CALCULATOR_H
#define CRC_CALCULATOR_H
#include "perry_common.h"
#include <cstdint>
#include <variant>
#include <stdexcept>

enum class CRCModel : uint8_t
{
    CRC8  = 0,
    CRC32 = 1
};

extern uint8_t calcCRC8(const std::vector<uint8_t>& data);
extern uint32_t calcCRC32(const std::vector<uint8_t>& data);

#endif // CRC_CALCULATOR_H
