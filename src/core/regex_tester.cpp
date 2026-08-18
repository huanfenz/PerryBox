#include "regex_tester.h"

namespace perry {

std::vector<RegexMatchResult> regexMatch(const std::string& pattern, const std::string& text,
                                         std::regex::flag_type flags, std::string* error)
{
    std::vector<RegexMatchResult> results;
    if (error) {
        error->clear();
    }

    if (pattern.empty() || text.empty()) {
        return results;
    }

    try {
        std::regex re(pattern, flags);

        auto it = std::sregex_iterator(text.begin(), text.end(), re);
        auto end = std::sregex_iterator();

        for (; it != end; ++it) {
            std::smatch match = *it;
            RegexMatchResult result;
            result.match = match.str();
            result.position = static_cast<int>(match.position());
            result.length = static_cast<int>(match.length());
            results.push_back(std::move(result));
        }
    } catch (const std::regex_error& e) {
        if (error) {
            *error = e.what();
        }
    }

    return results;
}

std::string regexReplace(const std::string& pattern, const std::string& text,
                         const std::string& replacement, std::regex::flag_type flags,
                         std::string* error)
{
    if (error) {
        error->clear();
    }

    if (pattern.empty() || text.empty()) {
        return text;
    }

    try {
        std::regex re(pattern, flags);
        return std::regex_replace(text, re, replacement);
    } catch (const std::regex_error& e) {
        if (error) {
            *error = e.what();
        }
        return text;
    }
}

const std::vector<RegexPreset>& regexPresets()
{
    static const std::vector<RegexPreset> presets = {
        { "-- 选择预设 --", "" },
        { "整数", R"(-?\d+)" },
        { "浮点数", R"(-?\d+\.?\d*)" },
        { "MAC地址", R"(([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2})" },
        { "IP地址(IPv4)", R"((25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?))" },
        { "IP地址(IPv6)", R"(([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|::|(:[0-9a-fA-F]{1,4}){1,7}|[0-9a-fA-F]{1,4}::([0-9a-fA-F]{1,4}:){0,5}[0-9a-fA-F]{1,4})" },
        { "邮箱", R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})" },
        { "手机号(中国)", R"(1[3-9]\d{9})" },
        { "身份证号", R"(\d{17}[\dXx])" },
        { "URL", R"(https?://[^\s]+)" },
        { "日期(yyyy-MM-dd)", R"(\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))" },
        { "时间(HH:mm:ss)", R"(([01]?\d|2[0-3]):[0-5]\d:[0-5]\d)" },
    };
    return presets;
}

}
