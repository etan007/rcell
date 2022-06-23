#ifndef _X_ENTRY_MANAGER
#define _X_ENTRY_MANAGER

#include "xEntry.h"
#ifndef _WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif
//#include <ext/hash_map>

#ifdef _WIN32
namespace std
{
    template<>
    struct hash<const char*>
    {
        size_t operator()(const char* val) const {
            UInt64 h = 0;
            for (; *val; ++val)
                h = 5 * h + *val;
            return size_t(h);
        }
    };
}
#else
namespace std {
    namespace tr1 {
        template<> 
        inline size_t hash<const char*>::operator()(const char* val) const
        {
            UInt64 h = 0;
            for ( ; *val; ++val)
                h = 5 * h + *val;
            return size_t(h);
        };
    }
}
#endif

struct equal_str
{
    bool operator()(const char* p1, const char*p2) const
    {
        return (strcmp(p1,p2)==0);
    }
};

struct equal_dword
{
    bool operator()(const UInt32 &a1, const UInt32 &a2)const
    {
        return a1==a2;
    }

};

struct equal_qword
{
    bool operator()(const UInt64 &a1, const UInt64 &a2)const
    {
        return a1==a2;
    }

};

struct xEntryCallBack
{
    virtual bool exec(xEntry *)=0;
    virtual ~xEntryCallBack() {}
};

template <typename keyT, typename valueT, typename equalT>
struct MultiHash : private xNoncopyable
{
    typedef std::tr1::unordered_multimap<keyT, valueT, std::tr1::hash<keyT>, equalT> multihashmap;
    //typedef __gnu_cxx::hash_multimap<keyT, valueT, __gnu_cxx::hash<keyT>, equalT> multihashmap;
    typedef typename multihashmap::const_iterator const_iter;
    typedef typename multihashmap::iterator iter;
    multihashmap ets;

    MultiHash()
    {
        ets.clear();
    }

    bool for_each(xEntryCallBack &c)
    {
        for (iter it=ets.begin(); it!=ets.end();)
        {
            iter temp = it++;
            if (!c.exec(temp->second)) return false;
        }
        return true;
    }
    UInt32 size()
    {
        return ets.size();
    }
};

struct xEntryMultiName : protected MultiHash<const char *, xEntry *, equal_str>
{
    xEntryMultiName()
    {
    }

    bool push(xEntry *e)
    {
        ets.insert(std::make_pair(e->name, e));
        return true;
    }

    void erase(xEntry *e)
    {
        ets.erase(e->name);
    }    
    void find(const char* name, xEntry** e)
    {
        if (!name) return;
        const_iter it = ets.find(name);
        if(it!=ets.end())
        {
            *e = it->second;
        }
    }
    xEntry* getEntryByName(const char* name)
    {
        if (!name) return NULL;
        const_iter it = ets.find(name);
        if(it!=ets.end())
             return it->second;
        return NULL;
    }
};

template <typename keyT, typename valueT, typename equalT, typename callbackT=xEntryCallBack, typename hashT=std::tr1::hash<keyT> >
//template <typename keyT, typename valueT, typename equalT, typename hashT=__gnu_cxx::hash<keyT> >
struct LimitHash : private xNoncopyable
{
    typedef std::tr1::unordered_map<keyT, valueT, hashT, equalT> hashmap;
    //typedef __gnu_cxx::hash_map<keyT, valueT, hashT, equalT> hashmap;
    typedef typename hashmap::const_iterator const_iter;
    typedef typename hashmap::iterator iter;
    hashmap ets;

    LimitHash()
    {
        ets.clear();
    }

    bool for_each(callbackT &c)
    {
        for (iter it=ets.begin(); it!=ets.end();)
        {
            iter temp = it++;
            if (!c.exec(temp->second)) return false;
        }
        return true;
    }
    UInt32 size()
    {
        return ets.size();
    }
};

template <int>
struct xEntryNone
{
    xEntryNone(){}
    bool push(xEntry* e) {return true;}
    void erase(xEntry* e){}
};

struct xEntryID : protected LimitHash<UInt32, xEntry *, equal_dword>
{
    xEntryID()
    {
    }

    bool push(xEntry *e)
    {
        const_iter it = ets.find(e->id);
        const_iter ix=ets.end();
        if (it ==ix)
        {
            ets.insert(std::pair<UInt32,xEntry *>(e->id, e));
            return true;
        }
        return false;
    }
    void erase(xEntry *e)
    {
        iter it = ets.find(e->id);
        if (it != ets.end())
        {
            ets.erase(it);
        }
    }
    xEntry* getEntryByID(UInt32 id)
    {
        const_iter it = ets.find(id);
        if(it!=ets.end())
             return it->second;
        return NULL;
    }
    void find(UInt32 id, xEntry** e)
    {
        const_iter it = ets.find(id);
        if(it!=ets.end())
        {
            *e = (it->second);
        }
    }
};

struct xEntryName : protected LimitHash<const char *, xEntry *, equal_str>
{
    xEntryName()
    {
    }

    bool push(xEntry *e)
    {
        iter it = ets.find(e->name);
        if (it == ets.end())
        {
            ets.insert(std::make_pair(e->name, e));
            return true;
        }
        return false;
    }

    void erase(xEntry *e)
    {
        iter it = ets.find(e->name);
        if (it != ets.end())
        {
            ets.erase(it);
        }
    }    
    void find(const char* name, xEntry** e)
    {
        if (!name) return;
        const_iter it = ets.find(name);
        if(it!=ets.end())
        {
            *e = it->second;
        }
    }
    xEntry* getEntryByName(const char* name)
    {
        if (!name) return NULL;
        const_iter it = ets.find(name);
        if(it!=ets.end())
             return it->second;
        return NULL;
    }
};

struct xEntryTempID : protected LimitHash<UInt32, xEntry *, equal_dword>
{
    xEntryTempID()
    {
    }

    bool push(xEntry *e)
    {
        const_iter it = ets.find(e->tempid);
        if (it == ets.end())
        {
            ets.insert(std::pair<UInt32,xEntry *>(e->tempid, e));
            return true;
        }
        return false;
    }
    void erase(xEntry *e)
    {
        iter it = ets.find(e->tempid);
        if (it != ets.end())
        {
            ets.erase(it);
        }
    }
    xEntry* getEntryByTempID(UInt32 id)
    {
        const_iter it = ets.find(id);
        if(it!=ets.end())
             return it->second;
        return NULL;
    }
    void find(UInt32 id, xEntry** e)
    {
        const_iter it = ets.find(id);
        if(it!=ets.end())
        {
            *e = (it->second);
        }
    }
};

template <typename e1, typename e2 = xEntryNone<1>, typename e3 = xEntryNone<2> >
struct xEntryManager : protected e1, protected e2, protected e3
{
    xEntryManager(){}
    virtual ~xEntryManager(){}

    bool addEntry(xEntry *e)
    {
        if (e1::push(e))
        {
            if (e2::push(e))
            {
                if (e3::push(e))
                {
                    return true;
                }
                else
                {
                    e1::erase(e);
                    e2::erase(e);
                    return false;
                }
            }
            else
            {
                e1::erase(e);
                return false;
            }
        
        }
        return false;
    }
    
    void removeEntry(xEntry *e)
    {
        e1::erase(e);
        e2::erase(e);
        e3::erase(e);
    }

    bool for_each(xEntryCallBack &c)
    {
        return e1::for_each(c);
    }

    UInt32 size()
    {
        return e1::size();
    }
};


#endif
