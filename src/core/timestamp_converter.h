#ifndef TIMESTAMP_CONVERTER_H
#define TIMESTAMP_CONVERTER_H
#include <ctime>
#include <string>

namespace perry {
    bool isValidTimestamp(const std::string& input);
    bool isValidTimeStr(const std::string& input);
    std::string timestamp2LocaltimeStr(std::time_t timestamp);
    std::string timestamp2UtctimeStr(std::time_t timestamp);
    bool localtime2Timestamp(const std::string& timeString, std::time_t& out);
    bool utctime2Timestamp(const std::string& timeString, std::time_t& out);
}

#endif // TIMESTAMP_CONVERTER_H
