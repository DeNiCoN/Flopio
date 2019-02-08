#pragma once
#include <vector>
#include <string>
#include <set>

extern bool wildcardMath(const char *pat, const char *str);

extern bool shaderLoadHelperFunc(int shaderType, char* rawBuffer, int rawBufSize, unsigned int *ID);

extern void shaderValidateCheck(unsigned int shader);

extern std::vector<std::string> splitpath(const std::string& str, const std::set<char> delimiters);

constexpr size_t ct_strlen(const char* s) noexcept {return *s ? 1 + ct_strlen(s + 1) : 0;}

//MurmurHash2
extern constexpr unsigned int hash(const char * key, unsigned int len);