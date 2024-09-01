#ifndef TIMESTAMP_CONVERTER_H
#define TIMESTAMP_CONVERTER_H
#include "perry_common.h"
#include <iomanip>
#include <sstream>
#include <ctime>

namespace perry {
    // 时间戳转换为时间字符串，格式：YYYY-MM-DD HH:MM:SS
    static std::string timestamp2LocaltimeStr(std::time_t timestamp) {
        std::tm* tmPtr = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // 时间字符串转换为时间戳，格式：YYYY-MM-DD HH:MM:SS
    static std::time_t localtime2Timestamp(const std::string& timeString) {
        std::tm tm = {};
        std::istringstream iss(timeString);
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (iss.fail()) {
            throw std::invalid_argument("Invalid time format");
        }
        return std::mktime(&tm);
    }

    static std::string timestampToUtctimeStr(std::time_t timestamp) {
        // 使用 std::gmtime 将时间戳转换为 UTC 时间
        std::tm* tmPtr = std::gmtime(&timestamp);
        // 使用 stringstream 来格式化时间
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        // 返回格式化后的字符串
        return oss.str();
    }
}

#endif // TIMESTAMP_CONVERTER_H
