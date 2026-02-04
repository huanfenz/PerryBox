#ifndef PERRY_COMMON_H
#define PERRY_COMMON_H

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
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

#endif // PERRY_COMMON_H
