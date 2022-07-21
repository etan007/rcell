extern "C" {
#include "lua/lua.h"  
#include "lua/lauxlib.h"  
#include "lua/lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

#include "sol/sol.hpp"
#include "pb.h"
#include "dynamic_msg_mgr.h"
//#include "g3log/g3log.hpp"
//#include "g3log/logworker.hpp"
#include "automaton/core/io/io.h"
//extern void luabind_netserver(sol::state & lua);
//extern void luabind_kcpserver(sol::state & lua);
//extern void luabind_websocket(sol::state & lua);
//extern void luabind_eventloop(sol::state & lua);
//extern void luabind_mysql(sol::state & lua);
//extern void luabind_redis(sol::state & lua);
//extern void luabind_basepacket(sol::state & lua);
//extern void luabind_common(sol::state & lua);
//extern void luabind_csvpar(sol::state & lua);
//extern void luabind_json(sol::state & lua);
//extern void luabind_httpserver(sol::state & lua);
const std::string path_to_log_file = "./";
using namespace automaton::core::io;
void init_lua_pb(lua_State* L)
{
	luaL_requiref(L, "pb", luaopen_pb, 0);
	luaL_requiref(L, "pb.slice", luaopen_pb_slice, 0);
	luaL_requiref(L, "pb.buffer", luaopen_pb_buffer, 0);
	luaL_requiref(L, "pb.conv", luaopen_pb_conv, 0);
}

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();

	// bind c++ && lua
	//luabind_netserver(lua);
	//luabind_kcpserver(lua);
	//luabind_websocket(lua);
	//luabind_eventloop(lua);
	//luabind_mysql(lua);
	//luabind_redis(lua);
	//luabind_basepacket(lua);
	//luabind_common(lua);
	//luabind_csvpar(lua);
	//luabind_json(lua);
	//luabind_httpserver(lua);

	//lua.script_file(argv[1]);
	//init_lua_pb(lua.lua_state());

	/*if (luaL_dofile(lua.lua_state(), argv[1]) == 1) {
		if (lua_isstring(lua.lua_state(), -1)) {
			printf("load lua file error:%s\n", lua_tostring(lua.lua_state(), -1));
		}
	}*/
	// test-begin
	
	auto worker = g3::LogWorker::createLogWorker();
	auto handle = worker->addDefaultLogger("rcell", path_to_log_file);
	g3::initializeLogging(worker.get());
	
	dynamic_msg_mgr::getMe().build_schema("","");
	/*dynamic_msg_mgr::getMe().load("../pb/");*/
	auto msg_factory = dynamic_msg_mgr::getMe().get_factory();
	auto msg1 = msg_factory->new_message_by_name("improbable.Position");
	auto field_info =  msg1->get_field_info_by_tag(2);
	/*
	auto msg1 = msg_factory->new_message_by_name("PBWorkerCommandRequestOp");
	auto msg2 = msg_factory->new_message_by_name("PBWorkerWorkerAttributes");
	auto field_info =  msg1->get_field_info_by_tag(1);
	msg2->set_uint32(1, 1);
	msg2->set_repeated_blob(2, "222222");
	std::string aa = msg2->get_repeated_blob(2, 0);
	msg1->set_int64(1, 1);
	msg1->set_int64(2, 2);
	msg1->set_uint32(3, 3);
	msg1->set_uint32(4, 4);
	msg1->set_int64(5, 5);
	msg1->set_message(6, *msg2);
	std::string strjson;
	msg1->to_json(&strjson);
	LOG(INFO) << "PBWorkerCommandRequestOp to_json "<<strjson;
	std::string strmsg;
	msg1->serialize_message(&strmsg);
	LOG(INFO) << "PBWorkerCommandRequestOp serialize_message  " << strmsg;*/
	// test-end

	system("pause");
	return 0;
}