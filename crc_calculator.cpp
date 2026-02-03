#include "crc_calculator.h"

/* ================= CRC8 =================
 * CRC-8-ATM
 * Poly: 0x07
 * Init: 0x00
 * RefIn/RefOut: false
 * XorOut: 0x00
 */
static uint8_t calcCRC8(const std::vector<uint8_t>& data)
{
    uint8_t crc = 0x00;
    constexpr uint8_t poly = 0x07;

    for (uint8_t byte : data)
    {
        crc ^= byte;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/* ================= CRC32 =================
 * Standard CRC-32 (IEEE 802.3)
 * Poly: 0x04C11DB7 (reversed: 0xEDB88320)
 * Init: 0xFFFFFFFF
 * RefIn/RefOut: true
 * XorOut: 0xFFFFFFFF
 */
static uint32_t calcCRC32(const std::vector<uint8_t>& data)
{
    uint32_t crc = 0xFFFFFFFF;
    constexpr uint32_t poly = 0xEDB88320;

    for (uint8_t byte : data)
    {
        crc ^= byte;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
}

/* ================= 对外统一接口 ================= */
std::variant<uint8_t, uint32_t>
calcCRC(const std::vector<uint8_t>& nums, CRCModel model)
{
    switch (model)
    {
        case CRCModel::CRC8:
            return calcCRC8(nums);

        case CRCModel::CRC32:
            return calcCRC32(nums);

        default:
            throw std::invalid_argument("Unsupported CRC model");
    }
}
