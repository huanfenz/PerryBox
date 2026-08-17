#include "ascii_converter.h"
#include <iomanip>
#include <bitset>
#include <sstream>

namespace perry {

/****************************** 工具函数 ****************************************/

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
        case BaseEnum::BIN:
            oss << std::bitset<8>(num);
            break;
        default:
            throw std::out_of_range("base must be DEC, HEX or BIN");
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
        unsigned char ch = static_cast<unsigned char>(num);
        res.push_back(ch);
    }
    return res;
}

/* 数字数组 转 指定进制字符串 */
static std::string nums2BaseStr(std::vector<uint8_t>& req, BaseEnum base, bool hasPrefix)
{
    if ((base != BaseEnum::HEX) && (base != BaseEnum::DEC) && (base != BaseEnum::BIN)) {
        throw std::out_of_range("base must be DEC, HEX or BIN");
    }

    std::ostringstream oss;
    for (size_t i = 0; i < req.size(); ++i) {
        if (i != 0) {
            oss << " ";
        }
        if (base == BaseEnum::HEX && hasPrefix) {
            oss << "0x" << num2BaseStr(req[i], base);
        } else if (base == BaseEnum::BIN && hasPrefix) {
            oss << "0b" << num2BaseStr(req[i], base);
        } else {
            oss << num2BaseStr(req[i], base);
        }
    }
    return oss.str();
}

/****************************** 对外接口 ****************************************/

AsciiConversionResult asciiConvert(const std::string& input, BaseEnum from, bool hasPrefix)
{
    AsciiConversionResult res;

    std::vector<uint8_t> nums;
    switch (from) {
        case BaseEnum::ASCII:
            nums = asciiStr2Nums(input);
            break;
        case BaseEnum::HEX:
            if (!isValidBaseStr(input, BaseEnum::HEX)) {
                return res;
            }
            nums = baseStr2Nums(input, BaseEnum::HEX);
            break;
        case BaseEnum::DEC:
            if (!isValidBaseStr(input, BaseEnum::DEC)) {
                return res;
            }
            nums = baseStr2Nums(input, BaseEnum::DEC);
            break;
        case BaseEnum::BIN:
            if (!isValidBaseStr(input, BaseEnum::BIN)) {
                return res;
            }
            nums = baseStr2Nums(input, BaseEnum::BIN);
            break;
        default:
            return res;
    }

    res.valid = true;
    res.charSize = nums.size();
    res.ascii = (from == BaseEnum::ASCII) ? input : nums2AsciiStr(nums);
    res.hex = nums2BaseStr(nums, BaseEnum::HEX, hasPrefix);
    res.dec = nums2BaseStr(nums, BaseEnum::DEC, hasPrefix);
    res.bin = nums2BaseStr(nums, BaseEnum::BIN, hasPrefix);
    return res;
}

}
