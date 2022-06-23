#ifndef _X_DEFINE_H_
#define _X_DEFINE_H_

#if 1
#pragma warning(disable:4099)//LNK4099
#pragma warning(disable:4996)
#pragma warning(disable:4200)
#pragma warning(disable:4355)
#endif


#include <stdint.h>
#include <mutex>


#define SAFE_DELETE_ARRAY(p) {if(p){delete []p;p=nullptr;}}


#ifndef _WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define Sleep(t)            usleep((int)t*1000)

#define GET_THREAD_ID_UE()    pthread_self()

#else

#include <winsock2.h>
#include <string>

#define snprintf            _snprintf

#define GET_THREAD_ID_UE()    GetCurrentProcessId()

#define bzero(p1,p2)        memset(p1,0,p2)
#define bcopy(p1,p2,p3)        memcpy((void*)(p2),(void*)(p1),p3)
#ifndef __attribute__
#define __attribute__(p)    ;
#endif
#define usleep(t)            Sleep(((unsigned int)t>1000)?((unsigned int)t/1000):1)

#endif


typedef int8_t                Int8;
typedef int16_t                Int16;
typedef int32_t                Int32;
typedef int64_t                Int64;

typedef uint8_t                UInt8;
typedef uint16_t            UInt16;
typedef uint32_t            UInt32;
typedef uint64_t            UInt64;

typedef    unsigned long        SM_KEY;


#ifndef _WIN32
typedef        Int32            SMHandle;
#else
typedef void*                SMHandle;
#endif


#define UInt8_MIN            ((UInt8)0)
#define UInt8_MAX            ((UInt8)0xff)           //255
#define UInt16_MIN            ((UInt16)0)
#define UInt16_MAX            ((UInt16)0xffff)       //65535
#define UInt32_MIN            ((UInt32)0)
#define UInt32_MAX            ((UInt32)0xffffffff)   //4294967295
#define UInt64_MIN            ((UInt64)0)
#define UInt64_MAX            ((UInt64)0xffffffffffffffff)   //18446744073709551615


#define MAX_NAMESIZE        32
#define MAX_DNAMESIZE        64
#define MAX_SNUIDSIZE        64
#define MAX_SGUIDSIZE        64
#define MAX_ROLENAMESIZE    18//创建角色名字长度
#define MAX_ROLE_SIZE        3
#define MAX_DBCHAR128_SIZE    128
#define MAX_DBCHAR256_SIZE    256
#define MAX_DBCHAR512_SIZE    512
#define SHA1_LEN            40


//无效的ID值
#define INVALID_ID            -1

//连接后等待发送消息的最长时间
#define MAX_WAIT_SEND_CMD_TIME 6


#define MAX_SERVER_EVENT    256
#define MAX_BUFFER_LEN        512
#define MAX_DBDATA_SIZE        (1<<22)


#define _TY(x) #x 
#define _S(x) _TY(x)


#ifndef M_PI
#  define M_PI                3.14159265358979323846
#endif

#ifndef M_PI_F
#  define M_PI_F            float(M_PI)
#endif


#define UE_MIN(a,b)            (((a)>=(b))?(b):(a))
#define UE_MAX(a,b)            (((a)>=(b))?(a):(b))

#include "xLog.h"
#include <string.h>
#include <list>
#include <map>
#include <vector>



// #define _LOCK_TEST_LOG_
#ifdef _LOCK_TEST_LOG_

extern std::map<std::string, int> gLockCountInfo;
#define ADD_LOCK_NUM(strLog) {\
gLockCountInfo[strLog]++;\
}
#define REDUCE_LOCK_NUM(strLog) {\
gLockCountInfo[strLog]--;\
}

#define LOCK_L(_lock) {\
char ___locklog[128]={0};\
sprintf(___locklog, "%s:%s,%d", typeid(*this).name(), #_lock, GET_THREAD_ID_UE());\
/*** if(gLockCountInfo[___locklog]>0){while(1){printf("lock error [%s][%s:%d] \n",___locklog,__FILE__,__LINE__);throw 1;}} ***/\
gLockCountInfo[___locklog]++;\
_lock.lock();\
}

#define UNLOCK_L(_lock) {\
char ___locklog[128] = { 0 };\
sprintf(___locklog, "%s:%s,%d", typeid(*this).name(), #_lock, GET_THREAD_ID_UE());\
/*** if(gLockCountInfo[___locklog]<1){while(1){printf("lock error [%s][%s:%d] \n",___locklog,__FILE__,__LINE__);throw 1;}} ***/\
gLockCountInfo[___locklog]--; \
_lock.unlock();\
}

#else

#define LOCK_L(_lock)        _lock.lock();
#define UNLOCK_L(_lock)        _lock.unlock();

#endif

#define MSG_NUM_MIN(a,b)    (((a)>=(b))?(b):(a))
#define MSG_NUM_MAX(a,b)    (((a)>=(b))?(a):(b))

#endif

