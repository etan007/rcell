#include "dynamic_msg_mgr.h"
#include <fstream>
#include <string>
#include "g3log/g3log.hpp"
#include "nlohmann/json.hpp"
#include "automaton/core/data/protobuf/protobuf_factory.h"
#include "automaton/core/data/protobuf/protobuf_schema.h"
#include "automaton/core/io/io.h"

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/util/json_util.h>

using automaton::core::data::protobuf::protobuf_schema;
using automaton::core::data::protobuf::protobuf_factory;
using automaton::core::data::schema;
using automaton::core::data::factory;
using automaton::core::io::get_file_contents;
using namespace google::protobuf;
//compiler::DiskSourceTree disk_source_tree;
//
//
//class MyMultiFileErrorCollector : public compiler::MultiFileErrorCollector
//{
//    virtual void AddError(const std::string& filename, int line, int column, const std::string& message) override
//    {
//        std::cout << "file: " << filename << ", line: " << line << ", col: " << column << ", message: " << message << std::endl;
//    }
//};

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
        //disk_source_tree.MapPath(path, "proto");
        //MyMultiFileErrorCollector error_collector;
        //compiler::Importer importer(&disk_source_tree, &error_collector);
        ////导入 proto 时，会自动导入所有依赖的 .proto 文件
        //compiler::Importer importer(&disk_source_tree, &error_collector);
        std::vector<std::string> schemas_filenames = j["schemas"];
        for (uint32_t z = 0; z < schemas_filenames.size(); ++z)
        {
           /* const FileDescriptor* file_descriptor = importer.Import(schemas_filenames[z]);
            if (!file_descriptor) {
                LOG(FATAL) << "Error while Import " << schemas_filenames[z];
            }*/
            auto& schema_name = schemas_filenames[z];
            ImportDependsfile(path, schema_name);
        }
        
    }
    return true;
}

void dynamic_msg_mgr::ImportDependsfile(const std::string& path, const std::string& schema_name)
{
    if (msgs_defs.find(schema_name) != msgs_defs.end())
    {
        return;
    }
    std::string file_content = get_file_contents((path + schema_name).c_str());
    msgs_defs[schema_name] = file_content;
    auto schema = new protobuf_schema(file_content);
    schemas.push_back(schema);

    // 添加依赖的文件
    auto fdp = schema->get_file_descriptor_proto();
    uint32_t dependencies_number = fdp->dependency_size();
    for (uint32_t i = 0; i < dependencies_number; ++i) {
        std::stringstream msg;
        msg << "Dependency <" << fdp->dependency(i) << ">";
        LOG(WARNING) << msg.str() << '\n';
        ImportDependsfile(path, fdp->dependency(i));
    }

    factory->import_schema(schema, schema_name, "");
}

dynamic_msg_mgr::dynamic_msg_mgr()
{
    factory = std::shared_ptr<protobuf_factory>(new protobuf_factory);
}

dynamic_msg_mgr::~dynamic_msg_mgr()
{
}
