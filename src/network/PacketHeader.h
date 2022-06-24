#ifndef __PACKET__HEADER_H__
#define __PACKET__HEADER_H__
#include "xDefine.h"
#pragma  pack(push)
#pragma  pack(1)

#define CODE_VALUE 0x88
#define MAX_PACK_SIZE 1024*1024
struct PacketHeader
{
    uint32_t   CheckCode;
    int32_t    nMsgID;
    int32_t    nSize;
    int32_t    nPacketNo;    //生成序号 = nMsgID^nSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->nMsgID^pHeader->nSize;
};
#define HEADER_LEN sizeof(PacketHeader)

#pragma  pack(pop)



#endif /* __PACKET__HEADER_H__*/
