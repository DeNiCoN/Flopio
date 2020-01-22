#ifndef HASHED_STRING_H
#define HASHED_STRING_H

#include "utils.h"

class HashedString
{
    private:
        const char* m_str;
        unsigned int length;
        unsigned int m_hashed;
    public:
        constexpr HashedString(const char* c_str)
            :length(std::char_traits<char>::length(c_str)), m_hashed(hash(c_str, length)), m_str(c_str)
        {}
        unsigned int getHashed() const { return m_hashed; }
        const char* getString() const { return m_str; }
};

#endif
