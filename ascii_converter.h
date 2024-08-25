#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H
#include "perry_common.h"

namespace perry {
enum class BaseEnum : uint8_t {
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16
};

enum class InputTypeEnum : uint8_t {
    ASCII = 0,
    DEC = 10,
    HEX = 16
};

class AsciiConverter {
public:
    AsciiConverter(std::string str, InputTypeEnum type);

    std::string getAsciiStr() const { return asciiStr; }
    std::string getHexStr() const { return hexStr; }
    std::string getDecStr() const { return decStr; }

private:
    std::string asciiStr;
    std::vector<uint8_t> nums;
    std::string hexStr;
    std::string decStr;
};
}

#endif // ASCII_CONVERTER_H
