#ifndef REGEX_TESTER_H
#define REGEX_TESTER_H

#include <string>
#include <vector>
#include <regex>

namespace perry {

struct RegexMatchResult {
    std::string match;
    int position = 0;
    int length = 0;
};

/* 正则匹配：返回全部匹配结果。
   pattern/text 为空时返回空结果；正则编译失败时返回空结果，
   并将错误描述写入 error（若非空）。 */
std::vector<RegexMatchResult> regexMatch(const std::string& pattern, const std::string& text,
                                         std::regex::flag_type flags, std::string* error = nullptr);

/* 正则替换：失败时原样返回 text，并将错误描述写入 error（若非空） */
std::string regexReplace(const std::string& pattern, const std::string& text,
                         const std::string& replacement, std::regex::flag_type flags,
                         std::string* error = nullptr);

/* 常用正则预设表：首项为占位项（pattern 为空），与界面下拉框顺序一致 */
struct RegexPreset {
    const char* name;
    const char* pattern;
};

const std::vector<RegexPreset>& regexPresets();

}

#endif // REGEX_TESTER_H
