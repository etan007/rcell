#ifndef RCELL_COMMON_H
#define RCELL_COMMON_H
#include "xDefine.h"

 

    /** 安全的释放一个指针内存 */
    #define RCELL_SAFE_RELEASE(p)								\
        if (p)											\
        {												\
        delete p;										\
        p = nullptr;									\
        }

    /** 安全的释放一个指针数组内存 */
    #define RCELL_RELEASE_ARRAY(p)						\
        if (p)											\
        {												\
        delete[] p;										\
        p = nullptr;									\
        }

    /** 定义服务器各组件状态 */
    enum COMPONENT_STATE
    {
        // 初始状态
        COMPONENT_STATE_INIT = 0,

        // 进程正在运行中
        COMPONENT_STATE_RUN = 1,

        // 进程开始关闭
        COMPONENT_STATE_SHUTTINGDOWN_BEGIN = 2,

        // 进程正在关闭
        COMPONENT_STATE_SHUTTINGDOWN_RUNNING = 3,

        // 进程关闭完成了
        COMPONENT_STATE_STOP = 4
    };

    /** 定义服务器各组件类别 */
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
