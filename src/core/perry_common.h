#ifndef PERRY_COMMON_H
#define PERRY_COMMON_H

#include <cstdint>
#include <string>
#include <vector>

enum class BaseEnum : uint8_t {
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16,
    ASCII = 0xFF,
};

extern std::vector<uint8_t> baseStr2Nums(const std::string& req, BaseEnum base);

/* 校验进制字符串格式：空格分隔、每组位数不超过该进制单字节上限。
   空串合法；ASCII 进制不做限制，恒为 true。 */
extern bool isValidBaseStr(const std::string& str, BaseEnum base);

#endif // PERRY_COMMON_H
