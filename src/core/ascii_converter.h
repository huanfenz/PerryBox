#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H
#include "perry_common.h"
#include <string>

namespace perry {

/* ASCII 转换结果：valid 为 false 表示输入格式校验失败，其余字段无效 */
struct AsciiConversionResult
{
    bool valid = false;
    std::string ascii;
    std::string hex;
    std::string dec;
    std::string bin;
    size_t charSize = 0;   // 字节的数量
};

/* 将 from 进制表示的 input 转换为各种进制的字符串 */
AsciiConversionResult asciiConvert(const std::string& input, BaseEnum from, bool hasPrefix);

}

#endif // ASCII_CONVERTER_H
