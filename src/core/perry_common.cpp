#include "perry_common.h"
#include <regex>
#include <sstream>
#include <stdexcept>

/* 指定进制字符串 转 数字数组 */
std::vector<uint8_t> baseStr2Nums(const std::string& req, BaseEnum base)
{
    if ((base != BaseEnum::HEX) && (base != BaseEnum::DEC) && (base != BaseEnum::BIN)) {
        throw std::out_of_range("base must be DEC, HEX or BIN");
    }

    std::vector<uint8_t> result;
    std::stringstream ss(req);
    std::string each;

    while (ss >> each) {
        uint8_t value =
                static_cast<uint8_t>(
                    std::stoi(each, nullptr, static_cast<int>(base))
                );
        result.push_back(value);
    }

    return result;
}

bool isValidBaseStr(const std::string& str, BaseEnum base)
{
    if (str.empty()) {
        return true;
    }
    if (base == BaseEnum::ASCII) {
        return true;
    }

    switch (base) {
        case BaseEnum::HEX: {
            // 匹配不带/带 0x 前缀的十六进制数，每个单词最多 2 个十六进制字符
            static const std::regex noPrefix(R"(^\s*[0-9a-fA-F]{1,2}(\s+[0-9a-fA-F]{1,2})*\s*$)");
            static const std::regex withPrefix(R"(^\s*0x[0-9a-fA-F]{1,2}(\s+0x[0-9a-fA-F]{1,2})*\s*$)");
            return std::regex_match(str, noPrefix) || std::regex_match(str, withPrefix);
        }
        case BaseEnum::DEC: {
            // 匹配以空格分隔的十进制数，每个数最多3位数字
            static const std::regex pattern(R"(^\s*\d{1,3}(\s+\d{1,3})*\s*$)");
            if (!std::regex_match(str, pattern)) {
                return false;
            }
            // 逐个检查数字是否在0到255之间
            std::istringstream stream(str);
            std::string word;
            while (stream >> word) {
                int number = std::stoi(word);
                if (number < 0 || number > 255) {
                    return false;
                }
            }
            return true;
        }
        case BaseEnum::BIN: {
            // 匹配不带/带 0b 前缀的二进制数，每个单词最多 8 个二进制字符
            static const std::regex noPrefix(R"(^\s*[01]{1,8}(\s+[01]{1,8})*\s*$)");
            static const std::regex withPrefix(R"(^\s*0b[01]{1,8}(\s+0b[01]{1,8})*\s*$)");
            return std::regex_match(str, noPrefix) || std::regex_match(str, withPrefix);
        }
        default:
            return false;
    }
}
