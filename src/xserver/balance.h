#ifndef __BALANCE_HEAD__
#define __BALANCE_HEAD__

#include <list>
//#include "util.h"
using std::list;


struct _SWeight
{
    _SWeight(uint32_t id) :id(id), weight(0)
    {
    }

    uint32_t id = 0;
    uint32_t weight = 0;
};


//负载均衡数据管理器
class CBalance
{
public:
    CBalance();
    ~CBalance();

public:
    //新增一个id,id必须>=0
    void AddNewId(uint32_t id);
    //增加权重数据
    void AddWeight(uint32_t id, uint32_t weight);
    //批量更新权重数据,例如:其他服务器汇报自己的负载情况给cwmd
    //todo
    //获取权重最小的id
    uint32_t GetLestWeightId();
    //随机获取一个id
    uint32_t GetRandomId();

    bool IsHas(uint32_t id)
    {
        for (auto& w : m_weights)
        {
            if (w->id == id)
                return true;
        }
        return false;
    }
private:
    list<_SWeight*> m_weights;

};



#endif


