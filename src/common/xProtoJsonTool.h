#pragma once


#include "json/json.h"
#include "google/protobuf/message.h"


#ifdef _WIN32
unsigned char __cdecl charEncrypt(int data=0);
unsigned char __cdecl charUnEncrypt(int data = 0);
int __cdecl base64_decode(char *base64 = 0, char *bindata = 0, int binlength = 0);
int __cdecl base64_encode(char *bindata = 0, char *base64 = 0, int binlength = 0);
#else
unsigned char charEncrypt(int data = 0);
unsigned char charUnEncrypt(int data = 0);
int base64_decode(char *base64 = 0, char *bindata = 0, int binlength = 0);
int base64_encode(char *bindata = 0, char *base64 = 0, int binlength = 0);
#endif


Json::Value Protobuf2Json_tool(const google::protobuf::Message &msg);

void _field2json(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor *field, size_t index, Json::Value &jsonValue);



#define PROTO_2_JSON_STRING(msg,str){\
    Json::Value cos = Protobuf2Json_tool(msg);\
    Json::FastWriter writer;\
    str = writer.write(cos);\
}






