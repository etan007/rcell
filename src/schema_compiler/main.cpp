#include <iostream>
#include <stdlib.h>

#include "pb/dynamic_msg_mgr.h"
#include "automaton/core/io/io.h"
#include "cmdline.h"

const std::string path_to_log_file = "./";
using namespace automaton::core::io; 
using std::string; 
 

bool InitWorkLog()
{
	static bool bInitLog = false;
	if(!bInitLog)
	{
		auto worker = g3::LogWorker::createLogWorker();
		auto handle = worker->addDefaultLogger("worker", path_to_log_file);
		g3::initializeLogging(worker.get());
		bInitLog = true;
	}
	
	return true;
}
bool build_schema(const std::string& path,const std::string& out_path)
{
	InitWorkLog();
	return dynamic_msg_mgr::getMe().build_schema(path,out_path);
}

 

int main(int argc, char* argv[])
{
   // Create a command line parser.
    cl::Cmdline cli("Example", "A short description");
	//--schema_path="D:/code/spatial/schema"
	//--core_path="D:/code/spatial/build/dependencies/schema/standard_library"
	//--bundle_out="D:/code/spatial/build/assembly/schema/schema.sb"
	//--bundle_json_out="D:/code/spatial/build/assembly/schema/schema.json"
	//--load_all_schema_on_schema_path  
    string schema_path,core_path,bundle_out,bundle_json_out;
	bool load_all_schema_on_schema_path;
	cli.Add("schema_path","",cl::Multiple::yes|cl::Arg::optional,cl::Var(schema_path));
	cli.Add("core_path","",cl::Multiple::yes|cl::Arg::optional,cl::Var(core_path));
	cli.Add("bundle_out","",cl::Multiple::yes|cl::Arg::optional,cl::Var(bundle_out));
	cli.Add("bundle_json_out","",cl::Multiple::yes|cl::Arg::optional,cl::Var(bundle_json_out));
	cli.Add("load_all_schema_on_schema_path","",cl::Multiple::yes|cl::Arg::optional,cl::Var(load_all_schema_on_schema_path));
	
    auto const result = cli.Parse(argv + 1, argv + argc);
    cli.PrintDiag();
    if (!result) {
        cli.PrintHelp();
    	LOG(WARNING) << "parse params err! " ;
        return -1;
    }

    // The command line has been successfully parsed.
    // Now do something useful...
	std::cout<<"schema_path: "<< schema_path << std::endl;
	std::cout<<"core_path: "<< core_path << std::endl;
	std::cout<<"bundle_out: "<< bundle_out << std::endl;
	std::cout<<"bundle_json_out: "<< bundle_json_out << std::endl;
	if(!build_schema(schema_path,""))
	{
		LOG(WARNING) << "build_schema err! " ;
		return -1;
	}
	
	return 0;
}