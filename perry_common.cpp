#include "perry_common.h"

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
