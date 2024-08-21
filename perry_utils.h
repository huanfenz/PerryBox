#ifndef PERRYUTILS_H
#define PERRYUTILS_H
#include "perry_common.h"
#include <vector>

namespace perry {
    enum class BaseEnum : uint8_t {
        BASE_BIN = 2,
        BASE_OCT = 8,
        BASE_DEC = 10,
        BASE_HEX = 16
    };

    extern std::vector<uint8_t> ascii2Nums(const std::string& req);
    extern std::string nums2StrByBase(std::vector<uint8_t>& req, BaseEnum base);
    extern std::vector<uint8_t> hexsStr2Nums(const std::string& req);
    extern std::vector<uint8_t> decsStr2Nums(const std::string& req);
    extern std::string nums2Ascii(const std::vector<uint8_t>& nums);
}

#endif // PERRYUTILS_H
