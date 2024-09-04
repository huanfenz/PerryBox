#ifndef TIMESTAMP_CONVERTER_H
#define TIMESTAMP_CONVERTER_H
#include "perry_common.h"
#include <ctime>

namespace perry {
    bool isValidTimestamp(const std::string& input);
    bool isValidTimeStr(const std::string& input);
    std::string timestamp2LocaltimeStr(std::time_t timestamp);
    std::string timestamp2UtctimeStr(std::time_t timestamp);
    std::time_t localtime2Timestamp(const std::string& timeString);
    std::time_t utctime2Timestamp(const std::string& timeString);
}

#endif // TIMESTAMP_CONVERTER_H
