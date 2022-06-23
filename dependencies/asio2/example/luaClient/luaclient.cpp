#include "lua.hpp"
#include "sol/sol.hpp"
#include "TcpClient.h"
 

void open_luaClient(lua_State* L)
{
	sol::state::state_view state(L);
	
	state.new_usertype<TcpClient>("TcpClient",
		"Init", &TcpClient::Init,
		"Connect", &TcpClient::Connect,
		"Send", &TcpClient::Send,
		"CloseSocket", &TcpClient::CloseSocket,
		"PeekMsg", &TcpClient::PeekMsg,
		"OnDisconnectCB", &TcpClient::OnDisconnectCB,
		"OnConnectCB", &TcpClient::OnConnectCB,
		"OnRecvCB", &TcpClient::OnRecvCB
		);
}


extern "C" __declspec(dllexport)  int luaopen_luaClient(lua_State* L) {
	sol::state::state_view state(L);
	state.do_string("print aaaaa");
	std::cout << "luaopen_luaClient";
	open_luaClient(L);
	return 1;
	//return sol::stack::call_lua(
	//	L, 1, open_luaClient);
}



