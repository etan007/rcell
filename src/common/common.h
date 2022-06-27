#ifndef RCELL_COMMON_H
#define RCELL_COMMON_H
#include "xDefine.h"

 

/** ��ȫ���ͷ�һ��ָ���ڴ� */
#define RCELL_SAFE_RELEASE(p)					    \
    if (p)											\
    {												\
    delete p;										\
    p = nullptr;									\
    }

/** ��ȫ���ͷ�һ��ָ�������ڴ� */
#define RCELL_RELEASE_ARRAY(p)						\
    if (p)											\
    {												\
    delete[] p;										\
    p = nullptr;									\
    }

/** ��������������״̬ */
enum COMPONENT_STATE
{
    // ��ʼ״̬
    COMPONENT_STATE_INIT = 0,

    // ��������������
    COMPONENT_STATE_RUN = 1,

    // ���̿�ʼ�ر�
    COMPONENT_STATE_SHUTTINGDOWN_BEGIN = 2,

    // �������ڹر�
    COMPONENT_STATE_SHUTTINGDOWN_RUNNING = 3,

    // ���̹ر������
    COMPONENT_STATE_STOP = 4
};

/** ����������������� */
enum COMPONENT_TYPE
{
    UNKNOWN_COMPONENT_TYPE	= 0,
    CWORKER_TYPE			= 1,
    SWORKER_TYPE			= 2,
    CELLMGR_TYPE            = 3,
    BASEAPP_TYPE			= 4,
    RCELL_TYPE			    = 5,
    WATCHER_TYPE			= 6,
    TOOL_TYPE				= 7,
    COMPONENT_END_TYPE		= 8,
};
namespace CommonFunc
{
    UInt64          GetTickCount();
    void            Sleep(UInt64 nMilliseconds);
}
    
 

#endif
