#include "timestamp_converter.h"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <QDebug>
#include <regex>

namespace perry {
    bool isValidTimestamp(const std::string& input) {
        // 正则表达式：匹配1到10位的数字
        std::regex timestampPattern(R"(^\d{1,10}$)");

        // 如果正则不匹配，返回false
        if (!std::regex_match(input, timestampPattern)) {
            return false;
        }

        // 转换为整数并检查是否在uint32_t范围内
        uint64_t number = std::stoull(input);
        if (number > 4294967295) {
            return false;
        }

        return true;
    }

    bool isValidTimeStr(const std::string& input) {
        // 正则表达式：匹配格式为 YYYY-MM-DD HH:MM:SS
        std::regex pattern(R"(^(\d{4})-(\d{2})-(\d{2})\s+(\d{2}):(\d{2}):(\d{2})$)");
        std::smatch match;

        // 如果格式不匹配，直接返回 false
        if (!std::regex_match(input, match, pattern)) {
            return false;
        }

        // 提取各部分日期和时间
        int year = std::stoi(match[1].str());
        int month = std::stoi(match[2].str());
        int day = std::stoi(match[3].str());
        int hour = std::stoi(match[4].str());
        int minute = std::stoi(match[5].str());
        int second = std::stoi(match[6].str());

        // 检查时间的合法性
        if (year < 0 || month < 1 || month > 12 || day < 1 || hour < 0 || hour > 23 ||
            minute < 0 || minute > 59 || second < 0 || second > 59) {
            return false;
        }

        // 每个月的天数限制
        int daysInMonth[] = {31, (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28,
                             31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // 检查天数是否在该月范围内
        if (day > daysInMonth[month - 1]) {
            return false;
        }

        // 检查1970年
        if (year < 1970) {
            return false;
        }

        return true;
    }

    std::string timestamp2LocaltimeStr(std::time_t timestamp)
    {
        std::tm* tmPtr = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string timestamp2UtctimeStr(std::time_t timestamp)
    {
        // 使用 std::gmtime 将时间戳转换为 UTC 时间
        std::tm* tmPtr = std::gmtime(&timestamp);
        // 使用 stringstream 来格式化时间
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        // 返回格式化后的字符串
        return oss.str();
    }

    std::time_t localtime2Timestamp(const std::string& timeString)\
    {
        std::tm tm = {};
        std::istringstream iss(timeString);
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (iss.fail()) {
            throw std::invalid_argument("Invalid time format");
        }
        return std::mktime(&tm);
    }

    std::time_t utctime2Timestamp(const std::string& timeString)
    {
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
