#ifndef _SHARE_MEM_ACCESS_OBJECT_H_
#define _SHARE_MEM_ACCESS_OBJECT_H_

#include "xDefine.h"

#include <assert.h>




#pragma pack(push, 1)
struct SMHead
{
    SM_KEY            m_Key;
    unsigned long    m_Size;
    UInt32            m_HeadVer;//�����̰汾
    SMHead() {
        m_Key = 0;
        m_Size = 0;
        m_HeadVer = 0;
    }
};
#pragma pack(pop)

#define        SHMEM_LOG_PATH    "./Log/ShareMemory.log"
/*
 *        �����ڴ���ʶ���
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
     *    ����ShareMem ���ʶ���(�´���)
     *
     *  SM_KEY    key        ���ʼ�ֵ
     *
     *    uint        Size    �����������ֽڸ���
     *
     */
    bool    Create(SM_KEY key, UInt32 Size);
    /*
     *    ���ٶ���
     */
    void    Destory();

    /*
     *        ����ShareMem ���ʶ���(�����´���)
     *        SM_KEY    key        ���ʼ�ֵ
     *
     *        uint        Size    �����������ֽڸ���
     *        
     */
    bool    Attach(SM_KEY, UInt32 Size);

    void    Clean(SM_KEY);
    /*
     *        ȡ������(������)
     */
    bool    Detach();

    /*
     *     ���������ָ��
     */
    char*    GetDataPtr()
    {
        return m_pDataPtr;
    }

    /*
     *    ��� ��СΪtSize �ĵ�tIndex ��smu������
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
     *    ����������ܴ�С
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
    //����
//     Int32                    m_CmdArg;

private:
    
    //ShareMemory    �ڴ��С
    Int32                m_Size = 0;
    // ShareMemory  ����ָ��
    char*                m_pDataPtr = nullptr;
    // ShareMemory    �ڴ�ͷָ��
    char*                m_pHeader = nullptr;
    // ShareMemory    ���    
    SMHandle            m_hold = 0;
};

#endif