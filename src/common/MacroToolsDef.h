/* ******************************************************************************
 *
 *   MacroToolsDef.h
 *   
 *   1.ȫ����Ҫʹ�õ��Ĺ��ߺ궨��
 *   
 *   author: lurence.gu
 */
#pragma once






// ���صȺŵĺ� ����ṹΪ�����������͵ĳ�Ա����
#define OPERATOR_EQUAL_TO(cname) \
cname &operator=(const cname &in) {\
    if (this == &in) {\
        return *this;\
    }\
    memcpy(this, &in, sizeof(cname));\
    return *this;\
}



























