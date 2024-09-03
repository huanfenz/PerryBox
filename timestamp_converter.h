#ifndef TIMESTAMP_CONVERTER_H
#define TIMESTAMP_CONVERTER_H
#include "perry_common.h"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <QDebug>

namespace perry {
    // 时间戳转换为时间字符串，格式：YYYY-MM-DD HH:MM:SS
    static std::string timestamp2LocaltimeStr(std::time_t timestamp) {
        std::tm* tmPtr = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static std::string timestamp2UtctimeStr(std::time_t timestamp) {
        // 使用 std::gmtime 将时间戳转换为 UTC 时间
        std::tm* tmPtr = std::gmtime(&timestamp);
        // 使用 stringstream 来格式化时间
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        // 返回格式化后的字符串
        return oss.str();
    }

    // 本地时间字符串转换为时间戳，格式：YYYY-MM-DD HH:MM:SS
    static std::time_t localtime2Timestamp(const std::string& timeString) {
        std::tm tm = {};
        std::istringstream iss(timeString);
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (iss.fail()) {
            throw std::invalid_argument("Invalid time format");
        }
        return std::mktime(&tm);
    }

    static std::time_t utctime2Timestamp(const std::string& timeString) {
        std::tm tm = {};
        std::istringstream iss(timeString);
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (iss.fail()) {
            throw std::invalid_argument("Invalid time format");
        }
        // 使用mktime将tm结构体转换为本地时间戳
        std::time_t localTime = std::mktime(&tm);
        // 获取本地时区和UTC的时差
        std::time_t utcOffset = std::difftime(localTime, std::mktime(std::gmtime(&localTime)));
        // 将本地时间戳加上时差，得到UTC时间戳
        return localTime + utcOffset;
    }
}

#endif // TIMESTAMP_CONVERTER_H
