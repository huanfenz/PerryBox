#include "perry_common.h"
#include "perry_utils.h"
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>

namespace perry {
    std::vector<uint8_t> ascii2Nums(const std::string& req)
    {
        std::vector<uint8_t> res;
        for (char c : req) {
            res.push_back(static_cast<uint8_t>(c));
        }
        return res;
    }

    /* 将一个 uint8_t 数字转换为指定进制的字符串 */
    static std::string num2BaseStr(uint8_t num, BaseEnum base)
    {
        std::ostringstream oss;
        switch (base) {
            case BaseEnum::BASE_BIN:
                for (int i = 7; i >= 0; --i) {
                    oss << ((num >> i) & 1);
                }
                break;
            case BaseEnum::BASE_OCT:
                oss << std::oct << static_cast<int>(num);
                break;
            case BaseEnum::BASE_DEC:
                oss << std::dec << static_cast<int>(num);
                break;
            case BaseEnum::BASE_HEX:
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(num);
                break;
            default:
                break;
        }
        return oss.str();
    }

    std::string nums2StrByBase(std::vector<uint8_t>& req, BaseEnum base)
    {
        std::ostringstream oss;
        for (size_t i = 0; i < req.size(); ++i) {
            if (i != 0) {
                oss << " ";
            }
            oss << num2BaseStr(req[i], base);
        }
        return oss.str();
    }

    static bool isHexStr(const std::string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!std::isxdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }

    static bool isDecStr(const std::string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }

    std::vector<uint8_t> hexsStr2Nums(const std::string& req)
    {
        std::vector<uint8_t> result;
        std::stringstream ss(req);
        std::string hexStr;

        while (ss >> hexStr) {
            if (!isHexStr(hexStr)) {
                break;
            }
            uint8_t value = static_cast<uint8_t>(std::stoi(hexStr, nullptr, 16));
            result.push_back(value);
        }

        return result;
    }

    std::vector<uint8_t> decsStr2Nums(const std::string& req)
    {
        std::vector<uint8_t> result;
        std::stringstream ss(req);
        std::string hexStr;

        while (ss >> hexStr) {
            if (!isDecStr(hexStr)) {
                break;
            }
            uint8_t value = static_cast<uint8_t>(std::stoi(hexStr, nullptr, 10));
            result.push_back(value);
        }

        return result;
    }

    std::string nums2Ascii(const std::vector<uint8_t>& nums)
    {
        std::string res;
        for (const uint8_t& num : nums) {
            res.push_back(static_cast<char>(num));
        }
        return res;
    }
}
