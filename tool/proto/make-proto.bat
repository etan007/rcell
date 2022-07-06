@echo off & color 0A

:: protoc程序名  protoc.exe 与gen.bat 在同一目录下 
set "PROTOC_EXE=protoc.exe"
:: .proto文件名，根据具体文件修改
set "PROTOC_FILE_NAME=protocol.proto"

set "PROTOC_PATH=%cd%"
set "CPP_OUT_PATH=%cd%"
set "PY_OUT_PATH =%cd%"
::生成.h和.cc
::"%PROTOC_PATH%\%PROTOC_EXE%" --proto_path="%PROTOC_PATH%" --cpp_out="%CPP_OUT_PATH%" "%PROTOC_PATH%\%PROTOC_FILE_NAME%"

cd %~dp0
for /f "delims=\" %%a in ('dir /b /a-d /o-d "*.proto"') do (
"%PROTOC_PATH%\%PROTOC_EXE%" --proto_path="%PROTOC_PATH%" --cpp_out="%CPP_OUT_PATH%" "%PROTOC_PATH%\%%a"
)
::拷贝pb
xcopy .\*.proto ..\..\pb\ /y
::拷贝到上层目录
xcopy .\*.h ..\..\src\pb\  /y
xcopy .\*.cc ..\..\src\pb\ /y

del .\*.h /f/s/q/a 
del .\*.cc /f/s/q/a 

pause