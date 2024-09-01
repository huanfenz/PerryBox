#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H
#include "perry_common.h"

namespace perry {
enum class BaseEnum : uint8_t {
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16,
    ASCII = 0xFF,
};

class AsciiConverter {
public:
    // 提供静态方法，返回对象
    static AsciiConverter& getInstance() {
        return instance;
    }

    static bool checkHexStrValid(const std::string str);
    static bool checkDecStrValid(const std::string str);

    void setAsciiStr(const std::string& str);
    void setHexStr(const std::string& str);
    void setDecStr(const std::string& str);
    std::string getAsciiStr() const { return asciiStr; }
    std::string getHexStr() const { return hexStr; }
    std::string getDecStr() const { return decStr; }
    uint32_t getCharSize() {
        return asciiStr.size();
    }

private:
    // 私有构造函数
    AsciiConverter() = default;
    // 禁止拷贝构造和赋值语句
    AsciiConverter(const AsciiConverter&) = delete;
    AsciiConverter& operator=(const AsciiConverter&) = delete;

    // 声明静态成员变量
    static AsciiConverter instance;

    std::string asciiStr;
    std::vector<uint8_t> nums;
    std::string hexStr;
    std::string decStr;
};
}

#endif // ASCII_CONVERTER_H
