#include "EntityFactory.h"


bool EntityFactory::Register(const std::string& strTypeName, std::function<Entity*(const std::string*)> pFunc)
{
    if (nullptr == pFunc)
    {
        XERR("EntityFactory::Register Failed! pFunc is nullptr! strTypeName:%s", strTypeName.c_str());
        return (false);
    }
    bool bReg = m_mapCreateFunction.insert(
        std::make_pair(strTypeName, pFunc)).second;
    if (!bReg)
    {
        XERR("EntityFactory::Register Failed! bReg is false! strTypeName:%s", strTypeName.c_str());
    }
    return (bReg);
}

Entity * EntityFactory::CreateEntity(const std::string & strTypeName, const std::string* pData)
{
    auto iter = m_mapCreateFunction.find(strTypeName);
    if (iter == m_mapCreateFunction.end())
    {
        XERR("EntityFactory::CreateEntity Failed! not find creator! strTypeName:%s", strTypeName.c_str());
        return (nullptr);
    }
    else
    {
        return (iter->second(pData));
    }
}