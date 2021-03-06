#include "xTools.h"
//#include "xXMLParser.h"


std::map<std::string, std::string> Tools::global;

bool Tools::initGlobal() {
    //xXMLParser p;
    //if (!p.parseDoc("config.xml"))
    //{
    //    XERR("[Tools]打开配置失败 config.xml");
    //    return false;
    //}
    //XDBG("[Tools] 加载 config.xml");

    //xmlNodePtr root = p.getRoot();
    //if (!root)
    //{
    //    XERR("[Tools] 配置文件错误 没有root节点");
    //    return false;
    //}

    //xmlNodePtr globalNode = p.getChild(root, "global");
    //if (!globalNode)
    //{
    //    XERR("[Tools] 没有global节点");
    //    return false;
    //}
    //std::string ss;
    //xmlNodePtr node = p.getChild(globalNode);
    //while (node)
    //{
    //    ss.clear();
    //    if (p.getPropValue(node, "value", &ss))
    //    {
    //        global[(char *)node->name] = ss;
    //        XDBG("[global] %s, %s", (char *)node->name, ss.c_str());
    //    }
    //    //else
    //        //XERR("[global] %s 没有 value 节点", (char *)node->name);

    //    node = p.getNext(node);
    //}
    return true;
}

UInt32 stringTok(std::string s, std::string k, std::vector<std::string> &v) {
    std::string::size_type len = s.length();
    std::string::size_type i = 0, j = 0;

    while (i < len)
    {
        i = s.find_first_not_of(k, i);
        if (i == std::string::npos) break;

        j = s.find_first_of(k, i);
        if (j == std::string::npos)
        {
            v.push_back(s.substr(i, s.length() - i));
            break;
        }
        else
        {
            v.push_back(s.substr(i, j - i));
            i = j + 1;
        }
    }

    return v.size();
}

UInt32 split(std::string str, std::string pattern, std::vector<std::string> &result) {
    std::string::size_type pos;
    //std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    UInt32 size = str.size();

    for (UInt32 i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result.size();
}

const char* printBinary(char* data, int dataLen) {
    std::string charData;
    snprintf(&(charData[0]), 1024, "%s", "0x");
    int pos = 2;
    for (int i = 0; i < dataLen; ++i)
    {
        snprintf(&(charData[pos]), 3, "%02x", *(char*)(data + i));
        pos += 2;
    }
    return charData.c_str();
}

#ifdef _WIN32
int strcasecmp(const char *s1, const char *s2) {
    while (toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
        if (*s1++ == '\0') return 0;
    return(toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

int strncasecmp(const char *s1, const char *s2, register int n) {
    while (--n >= 0 && toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
        if (*s1++ == '\0')  return 0;
    return(n < 0 ? 0 : toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}
#endif
 


#include <iostream>
#include <vector>
#include <fstream>

int AcSaveBinaryFile(const char *filePath, const char *outData, const int maxLen) {

    if (!filePath) {
        XERR("LoadBinaryFile !filePath ");
        return 0;
    }

    if (!outData) {
        XERR("LoadBinaryFile !outData ");
        return 0;
    }

    std::ofstream ofs(filePath, std::ifstream::binary);
    if (!ofs) {
        XERR("LoadBinaryFile !ifs [%s]", filePath);
        return 0;
    }

    ofs.write(outData, maxLen);

    ofs.close();

    return maxLen;
}

int AcLoadBinaryFile(const char *filePath, char *outData, const int maxLen) {

    if (!filePath) {
        XERR("LoadBinaryFile !filePath ");
        return 0;
    }

    if (!outData) {
        XERR("LoadBinaryFile !outData ");
        return 0;
    }
    
    std::ifstream ifs(filePath, std::ifstream::binary);
    if (!ifs) {
        XERR("LoadBinaryFile !ifs [%s]", filePath);
        return 0;
    }

    // get pointer to associated buffer object
    std::filebuf* pbuf = ifs.rdbuf();
    if (!pbuf) {
        XERR("LoadBinaryFile !pbuf ");
        return 0;
    }

    // get file size using buffer's members
    std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
    pbuf->pubseekpos(0, ifs.in);

    if (maxLen < (int)size) {
        XERR("LoadBinaryFile maxLen < (int)size [maxLen:%d,size:%d]", maxLen, size);
        return 0;
    }

    // get file data
    pbuf->sgetn(outData, size);

    ifs.close();

    return (int)size;
}
