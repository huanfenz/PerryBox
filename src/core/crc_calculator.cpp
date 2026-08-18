#include "crc_calculator.h"

/* 指定位宽的掩码 */
static uint64_t crcMask(int width)
{
    return (width >= 64) ? ~uint64_t(0) : ((uint64_t(1) << width) - 1);
}

/* 数值按位反射 */
static uint64_t crcReflect(uint64_t value, int width)
{
    uint64_t res = 0;
    for (int i = 0; i < width; ++i) {
        if (value & 1) {
            res |= uint64_t(1) << (width - 1 - i);
        }
        value >>= 1;
    }
    return res;
}

const std::vector<CrcModel>& crcModels()
{
    static const std::vector<CrcModel> models = {
        /* CRC-8-ATM: Poly 0x07, Init 0x00, RefIn/RefOut false, XorOut 0x00 */
        { "CRC-8",  8, 0x07,       0x00,       false, false, 0x00 },
        /* CRC-16/MODBUS: Poly 0x8005, Init 0xFFFF, RefIn/RefOut true, XorOut 0x00 */
        { "CRC-16/MODBUS", 16, 0x8005, 0xFFFF, true,  true,  0x0000 },
        /* CRC-16/CCITT-FALSE: Poly 0x1021, Init 0xFFFF, RefIn/RefOut false, XorOut 0x00 */
        { "CRC-16/CCITT", 16, 0x1021, 0xFFFF, false, false, 0x0000 },
        /* CRC-32 (IEEE 802.3): Poly 0x04C11DB7, Init 0xFFFFFFFF, RefIn/RefOut true, XorOut 0xFFFFFFFF */
        { "CRC-32", 32, 0x04C11DB7, 0xFFFFFFFF, true,  true,  0xFFFFFFFF },
    };
    return models;
}

uint64_t crcCalc(const CrcModel& model, const std::vector<uint8_t>& data)
{
    const uint64_t topBit = uint64_t(1) << (model.width - 1);
    const uint64_t mask = crcMask(model.width);
    uint64_t crc = model.init & mask;

    for (uint8_t byte : data) {
        const uint8_t cur = model.refin ? static_cast<uint8_t>(crcReflect(byte, 8)) : byte;
        crc ^= static_cast<uint64_t>(cur) << (model.width - 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & topBit) {
                crc = (crc << 1) ^ model.poly;
            } else {
                crc <<= 1;
            }
            crc &= mask;
        }
    }

    if (model.refout) {
        crc = crcReflect(crc, model.width);
    }
    return (crc ^ model.xorout) & mask;
}
