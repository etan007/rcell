#include "dynamic_msg_mgr.h"
#include <fstream>
#include <string>
#include "g3log/g3log.hpp"
#include "nlohmann/json.hpp"
#include "automaton/core/data/protobuf/protobuf_factory.h"
#include "automaton/core/data/protobuf/protobuf_schema.h"
#include "automaton/core/io/io.h"

using automaton::core::data::protobuf::protobuf_schema;
using automaton::core::data::protobuf::protobuf_factory;
using automaton::core::data::schema;
using automaton::core::data::factory;
using automaton::core::io::get_file_contents;

bool dynamic_msg_mgr::load(const std::string& path)
{
    std::ifstream i(path + "config.json");
    if (!i.is_open()) {
        LOG(WARNING) << "Error while opening " << path << "config.json";
        return false;
    }
    else
    {
        i.seekg(0);
        nlohmann::json j;
        i >> j;
        i.close();
        
        std::vector<std::string> schemas_filenames = j["schemas"];
        for (uint32_t z = 0; z < schemas_filenames.size(); ++z)
        {
            std::string file_content = get_file_contents((path + schemas_filenames[z]).c_str());
            msgs_defs[schemas_filenames[z]] = file_content;
            schemas.push_back(new protobuf_schema(file_content));
            factory->import_schema(schemas.back(), "", "");
        }
        
    }
    return true;
}

dynamic_msg_mgr::dynamic_msg_mgr()
{
    factory = std::shared_ptr<protobuf_factory>();
}

dynamic_msg_mgr::~dynamic_msg_mgr()
{
}
