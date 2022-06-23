require "luaClient"
local driver = require"client_driver_n"

local socket = TcpClient:new()
----------------------------------------------------------------
--回调函数,不要修改参数
socket.OnDisconnectCB = function(code,msg)
	print("on OnDisconnectCB")
    --driver:OnDisconnectCB(code,msg)
end

socket.OnConnectCB = function(code,msg)
	print("OnConnectCB")
    --driver:OnConnectCB(code,msg)
end
socket.OnRecvCB = function(msg)
	print("OnRecvCB")
    driver.last = self.last..msg
end
driver.socket = socket
 
local ip = "127.0.0.1"
local port = 19025
driver:connect(ip, port)





----------------------------------------------------------------
--socket:Init(0)
--socket:Connect("127.0.0.1",19025)
---------------------------------
print([[
账号已创建并登录成功
usage: 输入quit退出或输入"包名"回车后,再输入包数据
	quit  退出程序
	"包名"
		key1 value1 key2 value2 ...
]])

print("输入命令:")

while true do
	driver:dispatch_package()    --客户端游戏应该在游戏主循环调用这个函数
	local cmd = socket.readstdin()
	if cmd then
		if cmd == "quit" then
			driver:send_request("quit")
		else
			print("input key value:")
			local str = socket.readstdin()
			local p = {}
			for k, v in string.gmatch(str, "(w+) (%w+)") do
				p[k] = v
			end
			driver:send_request(cmd, p)
		end
	else
		socket.usleep(100)
	end
end