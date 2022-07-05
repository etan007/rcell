#pragma once
#include "xSingleton.h"
#include "automaton/core/data/factory.h"
#include "automaton/core/data/schema.h"
 
class dynamic_msg_mgr:public xSingleton<dynamic_msg_mgr>
{
public:
    dynamic_msg_mgr();
    ~dynamic_msg_mgr();
    
    bool load(const std::string& path);
private:
  
    std::unordered_map<std::string, std::string> msgs_defs;
    std::vector<automaton::core::data::schema*> schemas;
    std::shared_ptr<automaton::core::data::factory> factory;
};
