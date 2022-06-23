#ifndef _SHARE_MEM_ACCESS_OBJECT_H_
#define _SHARE_MEM_ACCESS_OBJECT_H_

#include "xDefine.h"

#include <assert.h>




#pragma pack(push, 1)
struct SMHead
{
    SM_KEY            m_Key;
    unsigned long    m_Size;
    UInt32            m_HeadVer;//最后存盘版本
    SMHead() {
        m_Key = 0;
        m_Size = 0;
        m_HeadVer = 0;
    }
};
#pragma pack(pop)

#define        SHMEM_LOG_PATH    "./Log/ShareMemory.log"
/*
 *        共享内存访问对象
 *        ShareMemory    Access    Object
 */
class ShareMemAO
{    
public:
    ShareMemAO()
    {
        m_pDataPtr    =    0;
        m_hold        =    0;
        m_Size        =    0;
        m_pHeader    =    0;    
    }
    ~ShareMemAO(){};
    /*
     *    创建ShareMem 访问对象(新创建)
     *
     *  SM_KEY    key        访问键值
     *
     *    uint        Size    访问数据区字节个数
     *
     */
    bool    Create(SM_KEY key, UInt32 Size);
    /*
     *    销毁对象
     */
    void    Destory();

    /*
     *        附着ShareMem 访问对象(不是新创建)
     *        SM_KEY    key        访问键值
     *
     *        uint        Size    访问数据区字节个数
     *        
     */
    bool    Attach(SM_KEY, UInt32 Size);

    void    Clean(SM_KEY);
    /*
     *        取消附着(不销毁)
     */
    bool    Detach();

    /*
     *     获得数据区指针
     */
    char*    GetDataPtr()
    {
        return m_pDataPtr;
    }

    /*
     *    获得 大小为tSize 的第tIndex 个smu的数据
     */
    char*    GetTypePtr(UInt32 tSize, UInt32 tIndex)
    {
        assert(tSize>0);
        assert((Int32)(tSize * tIndex) < m_Size);
        if (tSize <= 0 || (Int32)tIndex >= m_Size) {
            return nullptr;
        }
        return m_pDataPtr+tSize*tIndex;
    }
    /*
     *    获得数据区总大小
     */
    UInt32    GetSize()
    {
        return m_Size;
    }

    bool    DumpToFile(char* FilePath);
    bool    MergeFromFile(char* FilePath);

    UInt32    GetHeadVer();
    void    SetHeadVer(UInt32 ver);

public:
    //命令
//     Int32                    m_CmdArg;

private:
    
    //ShareMemory    内存大小
    Int32                m_Size = 0;
    // ShareMemory  数据指针
    char*                m_pDataPtr = nullptr;
    // ShareMemory    内存头指针
    char*                m_pHeader = nullptr;
    // ShareMemory    句柄    
    SMHandle            m_hold = 0;
};

#endif