#include "ascii_converter.h"
#include <iomanip>
#include <regex>
#include <QDebug>

namespace perry {
#if 0
/* 判断一个单词是否是指定进制的字符串 */
static bool isBaseStr(const std::string& str, BaseEnum base)
{
    if (str.empty()) return false;

    bool(*judgeFunc)(char) = nullptr;
    switch (base) {
        case BaseEnum::DEC:
            judgeFunc = [](char c) -> bool { return std::isdigit(c); };
            break;
        case BaseEnum::HEX:
            judgeFunc = [](char c) -> bool { return std::isxdigit(c); };
            break;
        default:
            throw std::out_of_range("base must be DEC or HEX");
    }

    for (char c : str) {
        if (!judgeFunc(c)) {
            return false;
        }
    }
    return true;
}
#endif

/* 将一个 uint8_t 数字转换为指定进制的字符串 */
static std::string num2BaseStr(uint8_t num, BaseEnum base)
{
    std::ostringstream oss;
    switch (base) {
        case BaseEnum::DEC:
            oss << std::dec << static_cast<int>(num);
            break;
        case BaseEnum::HEX:
            oss << std::hex << std::setw(2) <<std::setfill('0')
                << static_cast<int>(num);
            break;
        default:
            throw std::out_of_range("base must be DEC or HEX");
    }
    return oss.str();
}

/* ASCII字符串 转 数字数组 */
static std::vector<uint8_t> asciiStr2Nums(const std::string& req)
{
    std::vector<uint8_t> res;
    for (char c : req) {
        res.push_back(static_cast<uint8_t>(c));
    }
    return res;
}

/* 数字数组 转 ASCII字符串 */
static std::string nums2AsciiStr(const std::vector<uint8_t>& nums)
{
    std::string res;
    for (const uint8_t& num : nums) {
        res.push_back(static_cast<char>(num));
    }
    return res;
}

/* 数字数组 转 指定进制字符串 */
static std::string nums2BaseStr(std::vector<uint8_t>& req, BaseEnum base)
{
    if ((base != BaseEnum::HEX) && (base != BaseEnum::DEC)) {
        throw std::out_of_range("base must be DEC or HEX");
    }

    std::ostringstream oss;
    for (size_t i = 0; i < req.size(); ++i) {
        if (i != 0) {
            oss << " ";
        }
        oss << num2BaseStr(req[i], base);
    }
    return oss.str();
}

/* 指定进制字符串 转 数字数组 */
static std::vector<uint8_t> baseStr2Nums(const std::string& req, BaseEnum base)
{
    if ((base != BaseEnum::HEX) && (base != BaseEnum::DEC)) {
        throw std::out_of_range("base must be DEC or HEX");
    }

    std::vector<uint8_t> result;
    std::stringstream ss(req);
    std::string each;

    while (ss >> each) {
#if 0
        if (!isBaseStr(each, base)) {
            break;
        }
#endif
        uint8_t value =
                static_cast<uint8_t>(
                    std::stoi(each, nullptr, static_cast<int>(base))
                );
        result.push_back(value);
    }

    return result;
}

// 定义静态成员变量
AsciiConverter AsciiConverter::instance;

bool AsciiConverter::checkHexStrValid(const std::string str)
{
    if (str.empty()) return true;

    // 匹配不带 0x 前缀的十六进制数，每个单词最多 2 个十六进制字符
    std::regex pattern1(R"(^\s*[0-9a-fA-F]{1,2}(\s+[0-9a-fA-F]{1,2})*\s*$)");
    // 匹配带 0x 前缀的十六进制数，每个单词最多 2 个十六进制字符
    std::regex pattern2(R"(^\s*0x[0-9a-fA-F]{1,2}(\s+0x[0-9a-fA-F]{1,2})*\s*$)");
    // 校验格式
    bool formatCorrect = std::regex_match(str, pattern1) || std::regex_match(str, pattern2);

    if (!formatCorrect) {
        return false;
    }

    return true;
}

bool AsciiConverter::checkDecStrValid(const std::string str)
{
    if (str.empty()) return true;

    // 匹配以空格分隔的十进制数，每个数最多3位数字
    std::regex pattern(R"(^\s*\d{1,3}(\s+\d{1,3})*\s*$)");
    bool formatCorrect = std::regex_match(str, pattern);

    if (!formatCorrect) {
        return false;
    }

    // 使用字符串流逐个检查数字是否在0到255之间
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

void AsciiConverter::setAsciiStr(const std::string& str) {
    asciiStr = str;
    nums = asciiStr2Nums(asciiStr);
    hexStr = nums2BaseStr(nums, BaseEnum::HEX);
    decStr = nums2BaseStr(nums, BaseEnum::DEC);
}

void AsciiConverter::setHexStr(const std::string& str) {
    hexStr = str;
    nums = baseStr2Nums(hexStr, BaseEnum::HEX);
    asciiStr = nums2AsciiStr(nums);
    decStr = nums2BaseStr(nums, BaseEnum::DEC);
}

void AsciiConverter::setDecStr(const std::string& str) {
    decStr = str;
    nums = baseStr2Nums(decStr, BaseEnum::DEC);
    asciiStr = nums2AsciiStr(nums);
    hexStr = nums2BaseStr(nums, BaseEnum::HEX);
}

}
