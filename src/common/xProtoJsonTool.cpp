#include "xProtoJsonTool.h"



char aTuvwxtulmnopqr[65] = "tuvwxTUlmnopqrs7YZabcdefghij8yz0123456VWXkABCDEFGHIJKLMNOPQRS9+/";

#ifdef _WIN32
unsigned char __cdecl charEncrypt(int data) {
    int dataa; // [esp+18h] [ebp+8h]

    dataa = aTuvwxtulmnopqr[data];
    if (dataa > 64 && dataa <= 90)
        return 155 - dataa;
    if (dataa > 96 && dataa <= 122)
        return dataa - 64;
    if (dataa > 47 && dataa <= 57)
        return dataa + 50;
    if (dataa == 43)
        return 119;
    if (dataa == 47)
        dataa = 121;
    return dataa;
}
unsigned char __cdecl charUnEncrypt(int data) {
    int i;
    if (data == 121) {
        data = 47;
    }
    else if (data == 119) {
        data = 43;
    }
    else if (data > 47 + 50 && data <= 57 + 50) {
        data = data - 50;
    }
    else if (data > 96 - 64 && data <= 122 - 64) {
        data = data + 64;
    }
    else if (data >= 155 - 90 && data < 155 - 64) {
        data = 155 - data;
    }
    for (i = 0; i < 64; i++) {
        if (data == aTuvwxtulmnopqr[i])
            return i;
    }
    return 254;
}
int __cdecl base64_decode(char *base64, char *bindata, int binlength) {
    int t = 0, x = 0, y = 0, i = 0;
    unsigned char c = 0;
    int g = 3;

    while (base64[x] != 0) {
        // 需要解码的数据对应的ASCII值对应base64_suffix_map的值
        c = charUnEncrypt(base64[x]);
        x++;
        if (c == 255) return -1;// 对应的值不在转码表中
        if (c == 253) continue;// 对应的值是换行或者回车
        if (c == 254) { c = 0; g--; }// 对应的值是'='
        t = (t << 6) | c; // 将其依次放入一个int型中占3字节
        if (++y == 4) {
            bindata[i++] = (unsigned char)((t >> 16) & 0xff);
            if (g > 1) bindata[i++] = (unsigned char)((t >> 8) & 0xff);
            if (g > 2) bindata[i++] = (unsigned char)(t & 0xff);
            y = t = 0;
        }
    }
    return i;
}
int __cdecl base64_encode(char *bindata, char *base64, int binlength) {
    int v3; // eax
    char *v4; // ebx
    int v5; // eax
    int v6; // ST0C_4
    char *v7; // ebx
    int v8; // eax
    int v9; // eax
    char *v10; // ebx
    int v11; // eax
    int v12; // eax
    char *v13; // ebx
    uint8_t current; // [esp+Bh] [ebp-Dh]
    uint8_t currenta; // [esp+Bh] [ebp-Dh]
    int j; // [esp+Ch] [ebp-Ch]
    int ja; // [esp+Ch] [ebp-Ch]
    int jb; // [esp+Ch] [ebp-Ch]
    int i; // [esp+10h] [ebp-8h]

    i = 0;
    j = 0;
    while (i < binlength)
    {
        v3 = j;
        ja = j + 1;
        v4 = &base64[v3];
        *v4 = charEncrypt((bindata[i] >> 2) & 0x3F);
        current = (bindata[i] << 4) & 0x30;
        if (i + 1 >= binlength)
        {
            v5 = ja;
            v6 = ja + 1;
            v7 = &base64[v5];
            *v7 = charEncrypt(current);
            base64[v6] = '=';
            v8 = v6 + 1;
            j = v6 + 2;
            base64[v8] = '=';
            break;
        }
        v9 = ja;
        jb = ja + 1;
        v10 = &base64[v9];
        *v10 = charEncrypt((bindata[i + 1] >> 4) | current);
        currenta = 4 * bindata[i + 1] & 0x3C;
        if (i + 2 >= binlength)
        {
            base64[jb] = charEncrypt(currenta);
            v11 = jb + 1;
            j = jb + 2;
            base64[v11] = 61;
            break;
        }
        base64[jb] = charEncrypt((bindata[i + 2] >> 6) | currenta);
        v12 = jb + 1;
        j = jb + 2;
        v13 = &base64[v12];
        *v13 = charEncrypt(bindata[i + 2] & 0x3F);
        i += 3;
    }
    base64[j] = 0;
    return j;
}
#else
unsigned char charEncrypt(int data) {
    int dataa; // [esp+18h] [ebp+8h]

    dataa = aTuvwxtulmnopqr[data];
    if (dataa > 64 && dataa <= 90)
        return 155 - dataa;
    if (dataa > 96 && dataa <= 122)
        return dataa - 64;
    if (dataa > 47 && dataa <= 57)
        return dataa + 50;
    if (dataa == 43)
        return 119;
    if (dataa == 47)
        dataa = 121;
    return dataa;
}
unsigned char charUnEncrypt(int data) {
    int i;
    if (data == 121) {
        data = 47;
    } else if (data == 119) {
        data = 43;
    } else if (data > 47 + 50 && data <= 57 + 50) {
        data = data - 50;
    } else if (data > 96 - 64 && data <= 122 - 64) {
        data = data + 64;
    } else if (data >= 155 - 90 && data < 155 - 64) {
        data = 155 - data;
    }
    for (i = 0; i < 64; i++) {
        if (data == aTuvwxtulmnopqr[i])
            return i;
    }
    return 254;
}
int base64_decode(char *base64, char *bindata, int binlength) {
    int t = 0, x = 0, y = 0, i = 0;
    unsigned char c = 0;
    int g = 3;

    while (base64[x] != 0) {
        // 需要解码的数据对应的ASCII值对应base64_suffix_map的值
        c = charUnEncrypt(base64[x]);
        x++;
        if (c == 255) return -1;// 对应的值不在转码表中
        if (c == 253) continue;// 对应的值是换行或者回车
        if (c == 254) { c = 0; g--; }// 对应的值是'='
        t = (t << 6) | c; // 将其依次放入一个int型中占3字节
        if (++y == 4) {
            bindata[i++] = (unsigned char)((t >> 16) & 0xff);
            if (g > 1) bindata[i++] = (unsigned char)((t >> 8) & 0xff);
            if (g > 2) bindata[i++] = (unsigned char)(t & 0xff);
            y = t = 0;
        }
    }
    return i;
}
int base64_encode(char *bindata, char *base64, int binlength) {
    int v3; // eax
    char *v4; // ebx
    int v5; // eax
    int v6; // ST0C_4
    char *v7; // ebx
    int v8; // eax
    int v9; // eax
    char *v10; // ebx
    int v11; // eax
    int v12; // eax
    char *v13; // ebx
    uint8_t current; // [esp+Bh] [ebp-Dh]
    uint8_t currenta; // [esp+Bh] [ebp-Dh]
    int j; // [esp+Ch] [ebp-Ch]
    int ja; // [esp+Ch] [ebp-Ch]
    int jb; // [esp+Ch] [ebp-Ch]
    int i; // [esp+10h] [ebp-8h]

    i = 0;
    j = 0;
    while (i < binlength) {
        v3 = j;
        ja = j + 1;
        v4 = &base64[v3];
        *v4 = charEncrypt((bindata[i] >> 2) & 0x3F);
        current = (bindata[i] << 4) & 0x30;
        if (i + 1 >= binlength) {
            v5 = ja;
            v6 = ja + 1;
            v7 = &base64[v5];
            *v7 = charEncrypt(current);
            base64[v6] = '=';
            v8 = v6 + 1;
            j = v6 + 2;
            base64[v8] = '=';
            break;
        }
        v9 = ja;
        jb = ja + 1;
        v10 = &base64[v9];
        *v10 = charEncrypt((bindata[i + 1] >> 4) | current);
        currenta = 4 * bindata[i + 1] & 0x3C;
        if (i + 2 >= binlength) {
            base64[jb] = charEncrypt(currenta);
            v11 = jb + 1;
            j = jb + 2;
            base64[v11] = 61;
            break;
        }
        base64[jb] = charEncrypt((bindata[i + 2] >> 6) | currenta);
        v12 = jb + 1;
        j = jb + 2;
        v13 = &base64[v12];
        *v13 = charEncrypt(bindata[i + 2] & 0x3F);
        i += 3;
    }
    base64[j] = 0;
    return j;
}
#endif

Json::Value Protobuf2Json_tool(const google::protobuf::Message &msg) {
    Json::Value jsonRet;

    const google::protobuf::Descriptor *d = msg.GetDescriptor();
    const google::protobuf::Reflection *ref = msg.GetReflection();
    if (!d || !ref)
        return jsonRet;

    std::vector<const google::protobuf::FieldDescriptor *> fields;
    ref->ListFields(msg, &fields);

    for (size_t i = 0; i != fields.size(); i++)
    {
        const google::protobuf::FieldDescriptor *field = fields[i];
        const std::string &name = (field->is_extension()) ? field->full_name() : field->name();

        if (field->is_repeated())
        {
            size_t count = ref->FieldSize(msg, field);
            if (!count)
                continue;

            for (size_t j = 0; j < count; j++)
                _field2json(msg, field, j, jsonRet[name][(int)j]);
        }
        else if (ref->HasField(msg, field))
            _field2json(msg, field, 0, jsonRet[name]);
        else
            continue;
    }

    return jsonRet;
}

void _field2json(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor *field, size_t index, Json::Value &jsonValue) {
    const google::protobuf::Reflection *ref = msg.GetReflection();
    const bool repeated = field->is_repeated();

    switch (field->cpp_type())
    {
#define _CONVERT(type, ctype, sfunc, afunc)\
        case google::protobuf::FieldDescriptor::type: {\
            const ctype value = (repeated)?\
                ref->afunc(msg, field, (int)index): \
                ref->sfunc(msg, field);\
            jsonValue = value;\
            break;\
                }

        _CONVERT(CPPTYPE_DOUBLE, double, GetDouble, GetRepeatedDouble);
        _CONVERT(CPPTYPE_FLOAT, double, GetFloat, GetRepeatedFloat);

    case google::protobuf::FieldDescriptor::CPPTYPE_INT64: {
        const google::protobuf::int64 value = (repeated) ?
        ref->GetRepeatedInt64(msg, field, (int)index) :
        ref->GetInt64(msg, field);
//         char tempStr[32] = { 0 };
//         sprintf(tempStr, "%lld", value);
//         jsonValue = tempStr;

        jsonValue = (Json::Int64)value;
        break;
    }

    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: {
        const google::protobuf::uint64 value = (repeated) ?
        ref->GetRepeatedUInt64(msg, field, (int)index) :
        ref->GetUInt64(msg, field);
//         char tempStr[32] = {0};
//         sprintf(tempStr,"%llu", value);
//         jsonValue = tempStr;

        jsonValue = (Json::UInt64)value;
        break;
    }
//         _CONVERT(CPPTYPE_INT64, google::protobuf::int64, GetInt64, GetRepeatedInt64);
//         _CONVERT(CPPTYPE_UINT64, google::protobuf::uint64, GetUInt64, GetRepeatedUInt64);

        _CONVERT(CPPTYPE_INT32, google::protobuf::int32, GetInt32, GetRepeatedInt32);
        _CONVERT(CPPTYPE_UINT32, google::protobuf::uint32, GetUInt32, GetRepeatedUInt32);
        _CONVERT(CPPTYPE_BOOL, bool, GetBool, GetRepeatedBool);
#undef _CONVERT
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
        std::string scratch;
        const std::string &value = (repeated) ?
            ref->GetRepeatedStringReference(msg, field, (int)index, &scratch) :
            ref->GetStringReference(msg, field, &scratch);
//         if (field->type() == google::protobuf::FieldDescriptor::TYPE_BYTES) {
//             char tempBuffer[65535] = { 0 };
//             base64_encode((char *)value.c_str(), tempBuffer, (int)value.length());
//             jsonValue = tempBuffer;
//         }
//         else
            jsonValue = value.c_str();
        break;
    }
    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
        const google::protobuf::Message& mf = (repeated) ?
            ref->GetRepeatedMessage(msg, field, (int)index) :
            ref->GetMessage(msg, field);
        jsonValue = Protobuf2Json_tool(mf);
        break;
    }
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
        const google::protobuf::EnumValueDescriptor* ef = (repeated) ?
            ref->GetRepeatedEnum(msg, field, (int)index) :
            ref->GetEnum(msg, field);

        jsonValue = ef->number();
        break;
    }
    default:
        break;
    }
    //if (!jf) throw j2pb_error(field, "Fail to convert to json");
    //return jsonRet;
}


