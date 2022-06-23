/*
 * xLanguage.cpp
 *
 *  Created on: 2013-7-12
 *      Author: qingruibin
 */

#include "xLanguage.h"
#include "xTools.h"
#include "xSingleton.h"
#include "xLog.h"
static const UInt8 STRUTS_INIT = 1;
static const UInt8 STRUTS_LOAD = 2;
static const UInt8 STRUTS_SELECT = 3;
static const UInt16 LINECOUNT = 4096;
#define SELCTNAME "select"
#define TYPESNAME "types"
void varToVal(int*pos, const char * var, const char* value, std::string* ret) {
    *pos = ret->find(var, *pos);
    if (*pos != -1 && *pos <= (int) ret->length()) {
        ret->replace(*pos, strlen(var), value);
    }
}
xLanguage::xLanguage() {
    struts = STRUTS_INIT;
    loadtypes();
}

xLanguage::~xLanguage() {
}
bool xLanguage::select(const char* type) {
    if (struts >= STRUTS_INIT) {
        struts = STRUTS_SELECT;
        char buff[LINECOUNT];
        if (checktype(type))
            strncpy(buff, type, LINECOUNT);
        else {
            XERR("[语言包]选择了错误的语言type:%s", type);
            return false;
        }
        std::string select = Tools::global["lan"];
        select.append(SELCTNAME);
        FILE* pf = fopen(select.c_str(), "w+");
        if (pf) {
            fwrite(buff, 1, strlen(buff), pf);
            fclose(pf);
            return true;
        }
    }
    return false;
}

void xLanguage::load() {
    if (struts < STRUTS_INIT)
        return;
    if (!loadtypes()) {
        XERR("[语言包]加在类型失败 typefile:%s", TYPESNAME);
        return;
    }
    XLOG("[语言包]开始加载..");
    int lineCont = 128;
    char buffer[128];
    std::string lan = Tools::global["lan"];
    if (lan.length() < 1) {
        XERR("[语言包]找不到lan参数 请在config.xml global节点中添加 ");
        return;
    }
    std::string select = lan;
    select.append(SELCTNAME);
    std::ifstream redsectlan;
    redsectlan.open(select.c_str(), std::ios::in);
    if (!redsectlan) {
        XERR("[语言包]找不到当前包文件(%s)", lan.c_str());
        return;
    }
    bzero(buffer, lineCont);
    redsectlan.getline(buffer, lineCont);
    if (strlen(buffer) < 1) {
        XERR("[语言包]未选择任何语言");
        return;
    }
    std::string path;
    path.append(lan);
    path.append(buffer);
    std::vector<std::string> content;
    if (readFile(path.c_str(), content)) {

        for (UInt16 i = 0; i < content.size(); ++i) {
            std::vector<std::string> strVec;
            stringTok(content[i], "=", strVec);
            if (strVec.size() > 2)
            {
                for(UInt16 i = 2; i < strVec.size(); i++)
                {
                    strVec[1].append("=");
                    strVec[1].append(strVec[i].c_str());
                }
            }
            if (strVec.size() > 1) {
                _lanlist[strVec[0]] = strVec[1];
                //printf("%s\n",strVec[1].c_str());
            }
        }
    } else {
        XERR("[语言包]无法找到文件%s", path.c_str());
        return;
    }
    struts = STRUTS_LOAD;
    XLOG("[语言包]加在语言包结束共 %d 条 ", _lanlist.size());
}

void xLanguage::reload() {
    XLOG("[语言包]启动重新加载..");
    _lanlist.clear();
    load();
    XLOG("[语言包]重新加载完毕..");
}

void xLanguage::getValue(std::string key, std::string* ret) {
    if (_lanlist.size() < 1) {
        *ret = "";
    }
    int beginpos = 0;

    std::lock_guard<std::mutex> guard(langauge_critical);
    std::map<std::string, std::string>::iterator it = _lanlist.find(key);
    if (it != _lanlist.end())
        *ret = (_lanlist[key]);
    else {
        *ret = (_lanlist["Language.Null"]);
        varToVal(&beginpos, "{strvalue}", key.c_str(), ret);
    }

}
void xLanguage::append(std::string key, std::string* ret) {
    if (_lanlist.size() < 1) {
        *ret = "";
    }
    int beginpos = 0;
    std::lock_guard<std::mutex> guard(langauge_critical);
    std::map<std::string, std::string>::iterator it = _lanlist.find(key);
    if (it != _lanlist.end())
        ret->append(_lanlist[key].c_str());
    else {
        std::string temp = (_lanlist["Language.Null"]);
        varToVal(&beginpos, "{strvalue}", key.c_str(), &temp);
        ret->append(temp.c_str());
    }
}
bool xLanguage::loadtypes() {
    types.clear();
    std::string path = std::string(Tools::global["lan"]);
    path.append(TYPESNAME);
    if (readFile(path.c_str(), types))
        if (types.size() > 0)
            return true;
    return false;
}
bool xLanguage::checktype(const char* type) {

    for (UInt8 i = 0; i < types.size(); ++i) {
        if (strncmp(type, types[i].c_str(), 5) == 0)
            return true;
    }
    return false;
}
bool xLanguage::readFile(const char * path, std::vector<std::string> &content) {
    std::ifstream out;
    out.open(path, std::ios::in);
    char buffer[LINECOUNT];
    if (out) {
        std::lock_guard<std::mutex> guard(langauge_critical);
        while (out.getline(buffer, LINECOUNT)) {
            int len = strlen(buffer);
            if (len > 1) {
                std::vector<std::string> commVec;
                split(buffer,"//",commVec);
                if (commVec.size() > 0) {
                    content.push_back(commVec[0].c_str());
                }
            }

        }
        out.clear();
        out.close();
        return true;
    }
    return false;
}
