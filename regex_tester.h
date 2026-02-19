#ifndef REGEX_TESTER_H
#define REGEX_TESTER_H

#include <string>
#include <vector>
#include <regex>

namespace perry {

struct RegexMatchResult {
    std::string match;
    int position;
    int length;
};

class RegexTester {
public:
    static RegexTester& getInstance() {
        return instance;
    }

    std::string getPattern() const { return pattern; }
    void setPattern(const std::string& p) { pattern = p; }

    std::string getTestText() const { return testText; }
    void setTestText(const std::string& t) { testText = t; }

    std::regex::flag_type getFlags() const { return flags; }
    void setFlags(std::regex::flag_type f) { flags = f; }

    bool validatePattern(std::string& errorMsg);
    std::vector<RegexMatchResult> match();
    std::string getResultString();
    std::string replace(const std::string& replacement);

    static std::string getPresetPattern(int index);

private:
    RegexTester() : flags(std::regex::ECMAScript) {}
    RegexTester(const RegexTester&) = delete;
    RegexTester& operator=(const RegexTester&) = delete;

    static RegexTester instance;

    std::string pattern;
    std::string testText;
    std::regex::flag_type flags;
    std::vector<RegexMatchResult> results;
};

}

#endif
