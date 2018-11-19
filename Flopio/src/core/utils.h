#pragma once
#include <vector>
#include <string>
#include <set>

extern bool wildcardMath(const char *pat, const char *str);

extern std::vector<std::string> splitpath(const std::string& str, const std::set<char> delimiters);