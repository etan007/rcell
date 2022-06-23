#ifndef _XENTRY
#define _XENTRY
#include "xDefine.h"
#include "xTools.h"
#include "xNoncopyable.h"

struct xEntryC
{
    public:
        xEntryC()
        {
            id = 0;
            bzero(name, sizeof(name));
            tempid = 0;
        }
        virtual ~xEntryC(){}

        UInt32 id;
        char name[MAX_NAMESIZE+1];
        UInt32 tempid;    
};

struct xEntry : public xEntryC, private xNoncopyable
{
    xEntry(){}
    virtual ~xEntry(){}
};
#endif

