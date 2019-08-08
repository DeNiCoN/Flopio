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
constexpr unsigned int hash(const char * key, unsigned int len)
{
	const unsigned int m = 0x5bd1e995;
	const unsigned int seed = 0;
	const int r = 24;

	unsigned int h = seed ^ len;

	const unsigned char * data = (const unsigned char *)key;
	unsigned int k = 0;

	while (len >= 4)
	{
		k = data[0];
		k |= data[1] << 8;
		k |= data[2] << 16;
		k |= data[3] << 24;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}
