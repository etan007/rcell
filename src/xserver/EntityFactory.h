#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include "xSingleton.h"
#include <iostream>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

//��ϸ��       https://www.cnblogs.com/bwar/p/9304261.html
//  ��         https://blog.csdn.net/heyuhang112/article/details/51729435
 
class Entity;
//ʵ�幤����
class EntityFactory : public xSingleton<EntityFactory>
{
public:

    EntityFactory() {}
    virtual ~EntityFactory() {}

    bool Register(const std::string& strTypeName, std::function<Entity*(const std::string*)> pFunc);

    Entity* CreateEntity(const std::string& strTypeName, const std::string* pData);
private:
    std::unordered_map<std::string, std::function<Entity*(const std::string*)>> m_mapCreateFunction;
};






////////////////////////////////////////////DynamicCreator////////////////////////////////////


template<typename T>
class DynamicCreator
{
public:
    struct Register
    {
        Register()
        {
            const char* strTypeName = typeid(T).name();
            m_sTypeName = ReadTypeName(strTypeName);
            printf("DynamicCreator::Register typeid(T).name:%s,m_sTypeName:%s\n", strTypeName, m_sTypeName.c_str());
            EntityFactory::getMe().Register(m_sTypeName, CreateObject);
        }

        static std::string ReadTypeName(const char * name)
        {
#ifndef __GNUC__
            const char * p = strstr(name, " ");
            if (p)
            {
                size_t prev_len = (size_t)(p - name);
                if (memcmp(name, "class", prev_len) == 0 ||
                    memcmp(name, "struct", prev_len) == 0 ||
                    memcmp(name, "enum", prev_len) == 0 ||
                    memcmp(name, "union", prev_len) == 0)
                {
                    p += 1;
                    return std::string(p);
                }
            }

            return std::string(name);
#else
            char * real_name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
            std::string real_name_string(real_name);
            free(real_name);
            return real_name_string;
#endif
        }
        inline void do_nothing()const { };
    public:
        std::string m_sTypeName;
    };

public:
    DynamicCreator()
    {
        XLOG("DynamicCreator::construct! strTypeName:%s", GetTypeName().c_str());
        m_oRegister.do_nothing();
    }
    virtual ~DynamicCreator() { m_oRegister.do_nothing(); };

    static const std::string& GetTypeName() {
        return m_oRegister.m_sTypeName;
    }
    static Entity* CreateObject(const std::string* pData)
    {
        XLOG("static Entity* DynamicCreator::CreateObject()! sTypeName::%s", GetTypeName().c_str());
        T* pT = nullptr;
        try
        {
            pT = new T();
            if (pData && !pData->empty())
            {
                pT->SetData(*pData);
            }
        }
        catch (std::bad_alloc& e)
        {
            return(nullptr);
        }
        return(pT);
    }

private:
    static Register m_oRegister;
};

template<typename T>
typename DynamicCreator<T>::Register DynamicCreator<T>::m_oRegister;

