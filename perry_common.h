#ifndef PERRY_COMMON_H
#define PERRY_COMMON_H

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/* ASCII字符串 转 数字数组 */
extern std::vector<uint8_t> asciiStr2Nums(const std::string& req);

#endif // PERRY_COMMON_H
