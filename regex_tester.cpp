#include "regex_tester.h"
#include <fmt/format.h>

namespace perry {

RegexTester RegexTester::instance;

bool RegexTester::validatePattern(std::string& errorMsg)
{
    if (pattern.empty()) {
        errorMsg = "正则表达式不能为空";
        return false;
    }

    try {
        std::regex re(pattern, flags);
        errorMsg = "";
        return true;
    } catch (const std::regex_error& e) {
        errorMsg = fmt::format("正则表达式错误: {}", e.what());
        return false;
    }
}

std::vector<RegexMatchResult> RegexTester::match()
{
    results.clear();

    if (pattern.empty() || testText.empty()) {
        return results;
    }

    try {
        std::regex re(pattern, flags);

        std::sregex_iterator it(testText.begin(), testText.end(), re);
        std::sregex_iterator end;

        while (it != end) {
            std::smatch match = *it;
            RegexMatchResult result;
            result.match = match.str();
            result.position = static_cast<int>(match.position());
            result.length = static_cast<int>(match.length());
            results.push_back(result);
            ++it;
        }
    } catch (const std::regex_error&) {
    }

    return results;
}

std::string RegexTester::getResultString()
{
    if (results.empty()) {
        return "无匹配结果";
    }

    std::string output;
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        output += fmt::format("[{}] \"{}\"\n", i + 1, r.match);
        output += fmt::format("    位置: {}, 长度: {}\n", r.position, r.length);
    }
    return output;
}

std::string RegexTester::replace(const std::string& replacement)
{
    if (pattern.empty() || testText.empty()) {
        return testText;
    }

    try {
        std::regex re(pattern, flags);
        return std::regex_replace(testText, re, replacement);
    } catch (const std::regex_error&) {
        return testText;
    }
}

std::string RegexTester::getPresetPattern(int index)
{
    static const std::vector<std::string> presets = {
        "",
        R"(-?\d+)",
        R"(-?\d+\.?\d*)",
        R"(([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2})",
        R"((25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?)\.(25[0-5]|2[0-4]\d|[01]?\d\d?))",
        R"(([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|::|(:[0-9a-fA-F]{1,4}){1,7}|[0-9a-fA-F]{1,4}::([0-9a-fA-F]{1,4}:){0,5}[0-9a-fA-F]{1,4})",
        R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})",
        R"(1[3-9]\d{9})",
        R"(\d{17}[\dXx])",
        R"(https?://[^\s]+)",
        R"(\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))",
        R"(([01]?\d|2[0-3]):[0-5]\d:[0-5]\d)",
    };

    if (index < 0 || index >= static_cast<int>(presets.size())) {
        return "";
    }

    return presets[index];
}

}
