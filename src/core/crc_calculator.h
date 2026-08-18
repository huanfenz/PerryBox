#ifndef CRC_CALCULATOR_H
#define CRC_CALCULATOR_H
#include "perry_common.h"
#include <cstdint>
#include <string>
#include <vector>

/* CRC 参数模型 */
struct CrcModel
{
    const char* name;   // 界面显示名称
    int width;          // 位宽（8/16/32）
    uint64_t poly;      // 多项式（不反转形式）
    uint64_t init;      // 初始值
    bool refin;         // 输入反射
    bool refout;        // 输出反射
    uint64_t xorout;    // 结果异或值
};

/* 内置 CRC 参数模型表（界面下拉框顺序与此一致） */
const std::vector<CrcModel>& crcModels();

/* 按参数模型计算 CRC，返回值未做额外格式化 */
uint64_t crcCalc(const CrcModel& model, const std::vector<uint8_t>& data);

#endif // CRC_CALCULATOR_H
