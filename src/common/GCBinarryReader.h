#pragma once

#include <string>
 

#include "xSingleton.h"


static std::string m_strConfigBasePath___;

class GCBinarryReader
{
public:
    GCBinarryReader(char *pBuffer, int maxSize)
        : m_iMaxSize(maxSize), m_pBuffer(pBuffer) {
    }
    ~GCBinarryReader() {}

private:
    size_t m_iIndexPos = 0;
    size_t m_iMaxSize = 0;
    char *m_pBuffer = nullptr;

public:
    static void InitConfigBasePath(const char *pConfigBasePath) {
        m_strConfigBasePath___ = pConfigBasePath;
    }

public:
    static std::string GetConfigFullPath(std::string fileName) {
        std::string retStr = m_strConfigBasePath___;
        return retStr + fileName;
    }

public:
    int ReadChar(char defaultV = 0) {
        if (m_iIndexPos + sizeof(char) > m_iMaxSize) {
            m_iIndexPos = m_iMaxSize;
            return defaultV;
        }

        char iRet = *(char*)&m_pBuffer[m_iIndexPos];
        m_iIndexPos += sizeof(char);
        return iRet;
    }
    int ReadShort(short defaultV = 0) {
        if (m_iIndexPos + sizeof(short) > m_iMaxSize) {
            m_iIndexPos = m_iMaxSize;
            return defaultV;
        }

        short iRet = *(short*)&m_pBuffer[m_iIndexPos];
        m_iIndexPos += sizeof(short);
        return iRet;
    }
    int ReadInt(int defaultV = 0) {
        if (m_iIndexPos + sizeof(int) > m_iMaxSize) {
            m_iIndexPos = m_iMaxSize;
            return defaultV;
        }

        int iRet = *(int*)&m_pBuffer[m_iIndexPos];
        m_iIndexPos += sizeof(int);
        return iRet;
    }
    float ReadFloat(float defaultV = 0.0f) {
        if (m_iIndexPos + sizeof(float) > m_iMaxSize) {
            m_iIndexPos = m_iMaxSize;
            return defaultV;
        }

        float iRet = *(float*)&m_pBuffer[m_iIndexPos];
        m_iIndexPos += sizeof(float);
        return iRet;
    }
    std::string ReadString() {
        short iLen = ReadShort();
        if (iLen <= 0) {
            return "";
        }

        if (m_iIndexPos + iLen > m_iMaxSize) {
            m_iIndexPos = m_iMaxSize;
            return "";
        }

        static std::string strTmp;
        strTmp.assign(&m_pBuffer[m_iIndexPos], iLen);
        m_iIndexPos += iLen;
        return strTmp;
    }
};

