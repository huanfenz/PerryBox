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
    static std::string convertToBase(uint8_t num, BaseEnum base) {
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
                oss << convertToBase(req[i], base);
            }
            return oss.str();
    }
}
