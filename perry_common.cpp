#include "perry_common.h"

/* ASCII字符串 转 数字数组 */
std::vector<uint8_t> asciiStr2Nums(const std::string& req)
{
    std::vector<uint8_t> res;
    for (char c : req) {
        res.push_back(static_cast<uint8_t>(c));
    }
    return res;
}
