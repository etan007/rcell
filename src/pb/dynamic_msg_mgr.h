#pragma once
#include "xSingleton.h"
#include "automaton/core/data/factory.h"
#include "automaton/core/data/schema.h"
 
class dynamic_msg_mgr:public xSingleton<dynamic_msg_mgr>
{
public:
    dynamic_msg_mgr();
    ~dynamic_msg_mgr();
    bool build_schema(const std::string& path,const std::string& out_path);
    // 加载协议配置路径比如(pb/)
    bool load(const std::string& path,const std::string& jsonpath);
    void import_dependsfile(const std::string& path,const std::string& filename);
    std::shared_ptr<automaton::core::data::factory> get_factory();
private:
  
    std::unordered_map<std::string, std::string> msgs_defs;
    std::vector<automaton::core::data::schema*> schemas;
    std::shared_ptr<automaton::core::data::factory> factory;
};
