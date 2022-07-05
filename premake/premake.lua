--solution "rcell"
workspace "rcell"
location "../"
configurations { "Debug", "Release" }
platforms {'x32', 'x64'}

-- global scope, all workspaces will receive these values
 if os.target() == "windows" then
    defines '_CRT_SECURE_NO_WARNINGS'
 end

 
-----------------------------------------------------------------------------
project "liblua"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		"../dependencies/lua/include",
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../dependencies/lua";
	files { codedir.."/*.h",codedir.."/*.hpp", codedir.."/*.c", codedir.."/*.cc", codedir.."/*.cpp"}
	removefiles {codedir .. "/lua.c"}
	removefiles {codedir .. "/onelua.c"}
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_liblua"
	filter "configurations:Release"
		targetname "r_liblua"
---------------------------------------------------------------------------------		
project "hiredis"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		--"../dependencies/lua/include",
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../dependencies/hiredis";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	removefiles {codedir .. "/test.c"}
	removefiles {codedir .. "/ssl.c"}
	removefiles {codedir .. "/examples/**"}
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_redis"
	filter "configurations:Release"
		targetname "r_redis"
-------------------------------------------------------------------------------------------
project "automaton"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		"../dependencies",
	}
	language "C++"
	cppdialect "C++17"
	kind "StaticLib"
	local codedir = "../dependencies/automaton";
    files { codedir .. "/**.h", codedir .. "/**.hpp", codedir .. "/**.c", codedir .. "/**.cc", codedir .. "/**.cpp" }
    local codedir = "../dependencies/automaton/core/io";
    files { codedir .. "/**.h", codedir .. "/**.hpp", codedir .. "/**.c", codedir .. "/**.cc", codedir .. "/**.cpp" }
    local codedir = "../dependencies/automaton/core/data";
    files { codedir .. "/**.h", codedir .. "/**.hpp", codedir .. "/**.c", codedir .. "/**.cc", codedir .. "/**.cpp" }
    local codedir = "../dependencies/automaton/core/data/protobuf";
    files { codedir .. "/**.h", codedir .. "/**.hpp", codedir .. "/**.c", codedir .. "/**.cc", codedir .. "/**.cpp" }
	 
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_automaton"
	filter "configurations:Release"
		targetname "r_automaton"
-------------------------------------------------------------------------------------------		
project "rcell"
	-- 工程生成目录
	location "../src"
	-- 附加包含目录
	includedirs{
		"../dependencies",
		"../dependencies/asio2/include",
		"../dependencies/asio2/3rd",
		"../dependencies/sol2/include",
		"../dependencies/lua",
		"../dependencies/hiredis",
		"../dependencies/json",
		"../src/common",
		"../src/network",
		"../src/db",
		"../src/utils",
		"../src/xserver",
		"../src/pb",
	}
	language "C++"
	cppdialect "C++17"
	--buildoptions {"-std=c++17"}
	--linkoptions {"-pthread"}
	kind "ConsoleApp"
	local codedir = "../src/common";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/network";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/db";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/utils";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/pb";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/script";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/xserver";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../dependencies/asio2/include";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../dependencies/json";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	
	libdirs{"../libs"}
	targetdir "../bin"
	filter "system:windows"
	    libdirs{"../libs/Windows"}
		links {'ws2_32'}
		links {'Iphlpapi'}
		links {'Psapi'}
		links {'Userenv'}
		links {'libprotobuf'}
	filter "system:not windows"
		links {'pthread'}
		links {'dl', 'rt'}
	filter "configurations:Debug"
		links {'d_liblua'}
		links {'d_redis'}
		links {'d_automaton'}
	filter "configurations:Release"
		links {'r_liblua'}
		links {'r_redis'}
		links {'r_automaton'}
	filter {}
-------------------------------------------------------------------------------------------------------------------------------------		
 project "WorkerSDK"
-- 工程生成目录
	location "../src"
-- 附加包含目录
includedirs {
    "../dependencies",
	"../dependencies/automaton",
    "../src",
	"../src/WorkerSDK",
}
language "C++"
cppdialect "C++17"
kind "SharedLib"
local codedir = "../src/WorkerSDK/improbable";
files { codedir .. "/**.h", codedir .. "/**.hpp", codedir .. "/**.c", codedir .. "/**.cc", codedir .. "/**.cpp",codedir .. "/Source.def" }


libdirs{"../libs"}
targetdir "../bin"

filter "system:windows"
        defines 'WorkerSDK_DLL_EXPORTS'
		links {'libprotobuf'}
		links {'g3log'}
filter "system:not windows"
filter "configurations:Debug"
		links {'d_automaton'}
filter "configurations:Release"
		links {'r_automaton'}
filter {}	 
	 
	