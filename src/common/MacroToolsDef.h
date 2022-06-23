/* ******************************************************************************
 *
 *   MacroToolsDef.h
 *   
 *   1.全局需要使用到的工具宏定义
 *   
 *   author: lurence.gu
 */
#pragma once






// 重载等号的宏 必须结构为基础数据类型的成员变量
#define OPERATOR_EQUAL_TO(cname) \
cname &operator=(const cname &in) {\
    if (this == &in) {\
        return *this;\
    }\
    memcpy(this, &in, sizeof(cname));\
    return *this;\
}



























