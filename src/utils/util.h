#pragma once

#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <string>
#include <functional>
#include <fstream>
 

 

const std::string pattern2 = "([\\s]*[\\d]+[\\s]*,[\\s]*[\\d]+[\\s]*)";
const std::string pattern3 = "([\\s]*[\\d]+[\\s]*,[\\s]*[\\d]+[\\s]*,[\\s]*[\\d]+[\\s]*)";


class Util
{
public:
    static void RegexMatch(const std::string& sMatch, const std::string& sRegex, std::vector<std::string>& sVec)
    {
        std::regex pattern(sRegex);
        const std::sregex_token_iterator end;
        for (std::sregex_token_iterator i(sMatch.begin(), sMatch.end(), pattern); i != end; ++i) {
            sVec.push_back(*i);
        }
    }

    static void SplitStringToVector(const std::string& str, int delim, std::vector<std::string>& tokens)
    {
        tokens.clear();
         auto start = str.find_first_not_of(delim, 0);       // 分割到的字符串的第一个字符
         auto position = str.find_first_of(delim, start);    // 分隔符的位置
         while (position != std::string::npos || start != std::string::npos) {
             tokens.emplace_back(std::move(str.substr(start, position - start)));// [start, position) 为分割下来的字符串
             start = str.find_first_not_of(delim, position);
             position = str.find_first_of(delim, start);
         }
    }

    static void SplitStringToVectorInt(const std::string& str, int delim, std::vector<int>& tokens)
    {
         tokens.clear();
         auto start = str.find_first_not_of(delim, 0);       // 分割到的字符串的第一个字符
         auto position = str.find_first_of(delim, start);    // 分隔符的位置
         while (position != std::string::npos || start != std::string::npos) {
             tokens.emplace_back(atoi(str.substr(start, position - start).c_str()));// [start, position) 为分割下来的字符串
             start = str.find_first_not_of(delim, position);
             position = str.find_first_of(delim, start);
         }
    }

	static void replaceAwithX(std::string &str,const std::string sa, const std::string sx) {
		int pos;
		pos = str.find(sa);
		while (pos != -1) {
			// str.length()求字符的长度，注意str必须是string类型
			str.replace(pos, sa.length(), sx);
			pos = str.find(sa);
		}
	}

    static bool IsNum(const std::string& str)
    {
        for (int i = 0; i < str.size(); i++)
        {
            int tmp = (int)str[i];
            if (tmp >= 48 && tmp <= 57)
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        return true;
    }


    static int getStrLenUtf8(const char* str, bool &isGoodString) {
        if (!str) return 0;
        std::string tempName = str;
        int len = (int)strlen(tempName.c_str());
        int ret = 0;

        for (const char* sptr = tempName.c_str(); (sptr - tempName.c_str()) < len && *sptr;) {
            unsigned char ch = (unsigned char)(*sptr);
            if (ch < 0x80) {

                if (ch >= '0' && ch <= '9') {

                }
                else if (ch >= 'A' && ch <= 'Z') {

                }
                else if (ch >= 'a' && ch <= 'z') {

                }
                else {
                    isGoodString = false;
                }

                sptr++;    // ascii
                ret++;
            }
            else if (ch < 0xc0) {
                isGoodString = false;
                sptr++;    // invalid char
            }
            else if (ch < 0xe0) {
                sptr += 2;
                ret++;
            }
            else if (ch < 0xf0) {
                sptr += 3;
                ret++;
            }
            else {
                // 统一4个字节
                sptr += 4;
                ret++;
            }
        }

        return ret;
    }

    //用于清理一个指针容器
    template <typename TP,
        template <typename ELEM,
        typename ALLOC = std::allocator<ELEM>
        > class TC
    >
    static void ClearContainer(TC<TP, std::allocator<TP> >& c1)
    {
        while (!c1.empty())
        {
            typename TC<TP>::iterator iter = c1.begin();
            delete *iter;
            *iter = NULL;
            c1.erase(iter);
        }
    }

    //用于清理一个map,第二个类型为指针
    template<typename T1, typename T2,
        template <class _Kty,
        class _Ty,
        class _Pr = std::less<_Kty>,
        class _Alloc = std::allocator<std::pair<const _Kty, _Ty> >
        > class M
    >
    static void ClearMap(M<T1, T2, std::less<T1>, std::allocator<std::pair<const T1, T2> > >& c1)
    {
        typename M<T1, T2>::iterator iter = c1.begin();
        for (; iter != c1.end(); ++iter)
        {
            delete iter->second;
            iter->second = NULL;
        }
        c1.clear();
    }

    
};
