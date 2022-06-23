CC=gcc
GXX=g++
DEBUG_FLAG = -D_SQL_DEBUG_ZYX -D_ZYX_DEBUG -D__LINUX__

LUA_DIR = ./src
LUA_SRCS = $(wildcard $(LUA_DIR)/*.c)
LUA_OBJECTS = $(patsubst %.c, %.o,$(LUA_SRCS))
LUA_CFLAGS = -Wall -DLUA_USE_LINUX -g3


ZLIB_DIR = ./zlib
ZLIB_SRCS = $(wildcard $(ZLIB_DIR)/*.c)
ZLIB_OBJECTS = $(patsubst %.c, %.o,$(ZLIB_SRCS))
ZLIB_CFLAGS = -g3 -m64 -D_LARGEFILE64_SOURCE=1 

DES_DIR = ./des
DES_SRCS = $(wildcard $(DES_DIR)/*.c)
DES_OBJECTS = $(patsubst %.c, %.o,$(DES_SRCS))
DES_CFLAGS = -g3 

HIREDIS_DIR = ./hiredis
HIREDIS_SRCS = $(wildcard $(HIREDIS_DIR)/*.c)
HIREDIS_OBJECTS = $(patsubst %.c, %.o,$(HIREDIS_SRCS))
HIREDIS_CFLAGS = -g3

DBOP_DIR = ./dbop
DBOP_SRCS = $(wildcard $(DBOP_DIR)/*.cpp)
DBOP_OBJECTS = $(patsubst %.cpp, %.o,$(DBOP_SRCS))
DBOP_CFLAGS = -g3

SHA1_DIR = ./sha1
SHA1_SRCS = $(wildcard $(SHA1_DIR)/*.c)
SHA1_OBJECTS = $(patsubst %.c, %.o,$(SHA1_SRCS))
SHA1_CFLAGS = -g3 
DEBUG_CXXFLAGS = -I./ -I/usr/include/libxml2 -I/usr/include/mysql -D_DEBUG -D_ALL_SUPER_GM $(DEBUG_FLAG) -g -Wno-unknown-pragmas --std=c++0x -Werror -Wno-deprecated --std=c++0x
DEBUG_CXXLFLAGS =-L/usr/lib64/ -lX -lxml2 -lpthread -lmysqlclient -llog4cxx -lboost_regex -lrt

DIST_CXXFLAGS = -I./ -I../ -I../base -I../base/dbop -I../util -I../base/recastnavigation/Recast/Include -I../base/recastnavigation/Detour/Include -I../Command -I/usr/include/libxml2 -I/usr/include/mysql -Wall -Wno-unknown-pragmas --std=c++0x -Werror  -Wno-deprecated -g3 #-O2 优化暂不打开
DIST_CXXLFLAGS =-L/usr/lib64/ -lX -lxml2 -lpthread -lmysqlclient -llog4cxx -lboost_regex -lrt

BASE_OBJECTS =MemMonitorTool.o CRedis.o ProcessLinux.o RedisConnectPool.o ShareMemAO.o ShareMemAPI.o SMUManager.o TimeCycleRecord.o TimeManager.o xDBConnPool.o xExecutionTime.o xFunctionTime.o xGUIDMaker.o xHttpClient.o xLanguage.o xLog.o xStringTool.o xThread.o xTime.o xTools.o xXMLParser.o 

OBJECTS =$(BASE_OBJECTS) $(LUA_OBJECTS) $(ZLIB_OBJECTS) $(DES_OBJECTS) $(DBOP_OBJECTS) $(HIREDIS_OBJECTS) $(SHA1_OBJECTS) $(JSON_OBJECTS)

BINPATH=../../lib/libXBase/
TARGET = $(BINPATH)libXBase.a
CXXLFLAGS=$(DEBUG_CXXLFLAGS) 
CXXFLAGS=$(DEBUG_CXXFLAGS)
.PHONY : all

all: debug

debug:$(TARGET)
$(TARGET):$(OBJECTS)
	mkdir -p $(BINPATH)include/
	cp $(wildcard ./*.h) $(BINPATH)include/
	
	mkdir -p $(BINPATH)include/src/
	cp $(wildcard ./src/*.h) $(BINPATH)include/src/
	
	mkdir -p $(BINPATH)include/zlib/
	cp $(wildcard ./zlib/*.h) $(BINPATH)include/zlib/
	
	mkdir -p $(BINPATH)include/des/
	cp $(wildcard ./des/*.h) $(BINPATH)include/des/
	
	mkdir -p $(BINPATH)include/hiredis/
	cp $(wildcard ./hiredis/*.h) $(BINPATH)include/hiredis/
	
	mkdir -p $(BINPATH)include/dbop/
	cp $(wildcard ./dbop/*.h) $(BINPATH)include/dbop/	
	
	mkdir -p $(BINPATH)include/sha1/
	cp $(wildcard ./sha1/*.h) $(BINPATH)include/sha1/
	
	ar rcu $(TARGET) $(OBJECTS)	
	ranlib $(TARGET)	
	
	
$(BASE_OBJECTS): %.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c -fPIC $< -o $@
$(LUA_OBJECTS):%.o: %.c
	$(CC) -c -fPIC $(LUA_CFLAGS) $< -o $@ 
	@echo build lua
$(ZLIB_OBJECTS):  %.o: %.c
	$(CC) -c -fPIC $(ZLIB_CFLAGS) $< -o $@ 
	@echo build zlib
$(DES_OBJECTS):  %.o: %.c
	$(CC) -c -fPIC $(DES_CFLAGS) $< -o $@
	@echo build des
$(HIREDIS_OBJECTS):  %.o: %.c
	$(CC) -c -fPIC $(HIREDIS_CFLAGS) $< -o $@
	@echo build hiredis
$(DBOP_OBJECTS):  %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -fPIC $(DBOP_CFLAGS) $< -o $@
	@echo build dbop
$(SHA1_OBJECTS):  %.o: %.c
	$(CC) -c -fPIC $(SHA1_CFLAGS) $< -o $@ 
	@echo build sha1
	

SRCS = $(BASE_OBJECTS:%.o=%.cpp)
DEPS = $(BASE_OBJECTS:%.o=.%.dep)
.PHONY:include
#include $(DEPS)
$(DEPS): .%.dep: %.cpp 
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$;sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$


.PHONY: clean
clean:
	rm -rf $(BASE_OBJECTS) $(TARGET) $(LUA_DIR)/*.o $(ZLIB_DIR)/*.o $(DES_DIR)/*.o $(HIREDIS_DIR)/*.o $(DBOP_DIR)/*.o $(SHA1_DIR)/*.o
	rm -rf .*.dep
	rm -rf ./*.a
