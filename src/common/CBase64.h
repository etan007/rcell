#pragma once

#include "xDefine.h"

class CBase64
{
public:
    static std::string base64_encode(unsigned const char*, unsigned int len);
    static std::string base64_decode(const std::string&s);
};
