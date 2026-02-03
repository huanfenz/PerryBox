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

extern std::variant<uint8_t, uint32_t>
    calcCRC(const std::vector<uint8_t>& nums, CRCModel model);

#endif // CRC_CALCULATOR_H
