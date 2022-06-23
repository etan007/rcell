/*
 * Language.h
 *
 *  Created on: 2013-7-12
 *      Author: qingruibin
 */

#ifndef LANGUAGE_H_
#define LANGUAGE_H_
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include "xDefine.h"
#ifdef _WIN32
#endif
 

class xLanguage {
public:
    xLanguage();
    void load();
    void reload();
    void getValue(std::string key, std::string *ret);
    void append(std::string key, std::string* ret);
    bool select(const char* type);

    virtual ~xLanguage();
private :
    bool loadtypes();
    bool checktype(const char* type);
    bool readFile(const char * path,std::vector<std::string> &content);
private:

    UInt8 struts;
    std::map<std::string, std::string> _lanlist;
    std::mutex langauge_critical;
    std::vector<std::string> types;

};

#endif /* LANGUAGE_H_ */
