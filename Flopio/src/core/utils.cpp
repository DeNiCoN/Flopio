#include "utils.h"
#include "Engine.h"


bool wildcardMath(const char *pat, const char *str) {
	int i, star;

new_segment:
	star = 0;
	if (*pat == '*') {
		star = 1;
		do { pat++; } while (*pat == '*');
	}

test_match:
	for (i = 0; pat[i] && (pat[i] != '*'); i++) {
		if (str[i] != pat[i]) {
			if (!str[i]) return 0;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return 0;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*') {
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return 1;
	if (i && pat[i - 1] == '*') return 1;
	if (!star) return 0;
	str++;
	goto test_match;
}

bool shaderLoadHelperFunc(int shaderType, char* rawBuffer, int rawBufSize, unsigned int *ID)
{
	static char infoLog[512];
	int success, id;
	ID = glCreateShader(GL_VERTEX_SHADER);
	if (!ID)
	{
		engine::logger << "Shader creation failed, bad enum value: " << shaderType << "\n";
		return false;
	}
	glShaderSource(ID, 1, &rawBuffer, &rawBufSize);
	glCompileShader(ID);
	// print compile errors if any
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		engine::logger << "SHADER COMPILATION FAILED\n" << infoLog << "\n";
		return false;
	};

	return true;
}

std::vector<std::string> splitpath(const std::string& str, const std::set<char> delimiters)
{
	std::vector<std::string> result;

	char const* pch = str.c_str();
	char const* start = pch;
	for (; *pch; ++pch)
	{
		if (delimiters.find(*pch) != delimiters.end())
		{
			if (start != pch)
			{
				std::string str(start, pch);
				result.push_back(str);
			}
			else
			{
				result.push_back("");
			}
			start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}
