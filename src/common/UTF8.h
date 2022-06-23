#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

 

#ifdef _WIN32
#include <windows.h>

inline string GbkToUtf8(const wchar_t *wstr) {
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if (str) delete[] str;
    return strTemp;
}

inline string Utf8ToGbk(const char *src_str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
     wchar_t* wszGBK = new wchar_t[len + 1];
     memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    string strRet((char*)wszGBK);
    if(wszGBK)delete[]wszGBK; wszGBK = nullptr;
    return strRet;
}

#else

#include <iconv.h>

inline int GbkToUtf8Ex(char *str_str, size_t src_len, char *dst_str, size_t dst_len) {
    iconv_t cd;

    cd = iconv_open("utf8", "gbk");
    if (cd == 0)
        return -1;

    char **pin = &str_str;
    char **pout = &dst_str;

    memset(dst_str, 0, dst_len);
    iconv(cd, pin, &src_len, pout, &dst_len);
    //if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
    //    return -1;
    iconv_close(cd);
    dst_str[0] = '\0';

    return 0;
}

inline string GbkToUtf8(const std::string &str_str) {
    char tmpData[65535] = { 0 };
    if (0 == GbkToUtf8Ex((char *)str_str.c_str(), str_str.size(), tmpData, 65535)) {
        return tmpData;
    }
    return "";
}


inline int Utf8ToGbkEx(char *src_str, size_t src_len, char *dst_str, size_t dst_len) {
    iconv_t cd;
    char **pin = &src_str;
    char **pout = &dst_str;

    cd = iconv_open("gbk", "utf8");
    if (cd == 0)
        return -1;
    memset(dst_str, 0, dst_len);
    iconv(cd, pin, &src_len, pout, &dst_len);
    //if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
    //    return -1;
    iconv_close(cd);
    dst_str[0] = '\0';

    return 0;
}

inline string Utf8ToGbk(const std::string &str_str) {
    char tmpData[65535] = { 0 };
    if (0 == Utf8ToGbkEx((char *)str_str.c_str(), str_str.size(), tmpData, 65535)) {
        return tmpData;
    }
    return "";
}

#endif

inline bool is_str_utf8(const char* str) {
    //UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned int nBytes = 0;
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0) {
            bAllAscii = false;
        }
        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD) {
                    nBytes = 6;
                }
                else if (chr >= 0xF8) {
                    nBytes = 5;
                }
                else if (chr >= 0xF0) {
                    nBytes = 4;
                }
                else if (chr >= 0xE0) {
                    nBytes = 3;
                }
                else if (chr >= 0xC0) {
                    nBytes = 2;
                }
                else {
                    return false;
                }
                nBytes--;
            }
        }
        else {
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80) {
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0) {
        return false;
    }
    if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
        return true;
    }
    return true;
}

inline bool is_str_gbk(const char* str) {
    //GBK可用1-2个字节编码,中文两个 ,英文一个
    unsigned int nBytes = 0;
    unsigned char chr = *str;
    bool bAllAscii = true; //如果全部都是ASCII,
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        chr = *(str + i);
        // 判断是否ASCII编码,如果不是,说明有可能是GBK
        if ((chr & 0x80) != 0 && nBytes == 0) {
            bAllAscii = false;
        }
        if (nBytes == 0) {
            if (chr >= 0x80) {
                if (chr >= 0x81 && chr <= 0xFE) {
                    nBytes = +2;
                }
                else {
                    return false;
                }
                nBytes--;
            }
        }
        else {
            if (chr < 0x40 || chr>0xFE) {
                return false;
            }
            nBytes--;
        }//else end
    }
    //违返规则
    if (nBytes != 0) {   
        return false;
    }
    //如果全部都是ASCII, 也是GBK
    if (bAllAscii) { 
        return true;
    }
    return true;
}

// 
// int main(void) {
//     char *src_str = "葡萄美酒夜光杯";
//     cout << "origin string: " << src_str << endl;
// 
// #ifdef _WIN32
//     // windows default is gbk
//     string dst_str = GbkToUtf8(src_str);
//     cout << "gbk to utf8: " << dst_str << endl;
// 
//     string str_utf8 = Utf8ToGbk(dst_str.c_str());
//     cout << "utf8 to gbk: " << str_utf8 << endl;
// #else
//     // unix default is utf8
//     char dst_gbk[1024] = { 0 };
//     Utf8ToGbk(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk));
//     cout << "utf8 to gbk: " << dst_gbk << endl;
// 
//     char dst_utf8[1024] = { 0 };
//     GbkToUtf8(dst_gbk, strlen(dst_gbk), dst_utf8, sizeof(dst_utf8));
//     cout << "gbk to utf8: " << dst_utf8 << endl;
// #endif
// 
//     return 0;
// }
// 
