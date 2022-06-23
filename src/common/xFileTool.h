#pragma once

#include "xDefine.h"




// ����·�� �� .. �� . ȥ�����㵽ȫ����·��
inline string MakePathToSort(string &path) {
    string makeStr = path;

    // ȥ�� "./"  ".\" �����ָ�ʽ

    while (true) {
        int pos = makeStr.find("\\./");
        if (pos >= 0) {
            makeStr.replace(pos, 2, "");
        }

        pos = makeStr.find("\\.\\");
        if (pos >= 0) {
            makeStr.replace(pos, 2, "");
        }

        pos = makeStr.find("/./");
        if (pos >= 0) {
            makeStr.replace(pos, 2, "");
        }

        pos = makeStr.find("/.\\");
        if (pos >= 0) {
            makeStr.replace(pos, 2, "");
        }

        int i1 = makeStr.find("\\./");
        int i2 = makeStr.find("\\.\\");
        int i3 = makeStr.find("/./");
        int i4 = makeStr.find("/.\\");

        if (i1 < 0 && i2 < 0 && i3 < 0 && i4 < 0) {
            break;
        }
    }

    do {
        int pos = makeStr.find("..");
        if (pos > 0) {
            // /a/b/c/../sdf/ad.exe
            string leftStr = makeStr.substr(0, pos - 1);// /a/b/c
            string rightStr = makeStr.substr(pos + 2, makeStr.size());// /sdf/ad.exe

            for (int i = leftStr.size() - 1; i >= 0; --i) {
                if (leftStr.at(i) == '\\' || leftStr.at(i) == '/') {
                    makeStr = leftStr.substr(0, i) + rightStr;// /a/b + /sdf/ad.exe
                    break;
                }
            }
        }

        pos = makeStr.find("..");
        if (pos <= 0) {
            break;
        }

    } while (true);

    path = makeStr;
    makeStr = "";

    for (size_t i = 0; i < path.size(); ++i) {
        if ('/' == path.at(i)) {
            makeStr += '\\';
        } else {
            makeStr += path.at(i);
        }
    }

    return path = makeStr;
}

// ��ȡ��ǰ�������Ŀ¼�ľ���Ŀ¼
// relativePath - "../.\\..//"
inline string MakeCurProcessRelativePath(const string &relativePath) {
    string mLauncherBasePath;

#ifdef _WIN32
    wchar_t runPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, runPath, MAX_PATH);

    char nchar[256] = { 0 };
    WideCharToMultiByte(CP_ACP, 0, runPath, -1, nchar, 256, NULL, NULL);

    mLauncherBasePath = nchar;
    mLauncherBasePath = mLauncherBasePath.substr(0, mLauncherBasePath.rfind('\\') + 1);
    // ����·�������·��ƴ��һ��
    mLauncherBasePath = mLauncherBasePath + relativePath;
    // ȥ��·���е����·�� ".." �� "."
    return MakePathToSort(mLauncherBasePath);
#else

    return relativePath;

#endif
}


// ��ȡ��ǰ�������Ŀ¼�ľ���Ŀ¼
// relativePath - "../.\\..//"
inline string MakeCurProcessRelativePathEx(const string &basePath, const string &relativePath) {
    if (basePath.empty()) {
        return MakeCurProcessRelativePath(relativePath);
    }


#ifdef _WIN32
    string mLauncherBasePath;
    mLauncherBasePath = basePath;
    mLauncherBasePath = mLauncherBasePath.substr(0, mLauncherBasePath.rfind('\\') + 1);
    // ����·�������·��ƴ��һ��
    mLauncherBasePath = mLauncherBasePath + relativePath;
    // ȥ��·���е����·�� ".." �� "."
    return MakePathToSort(mLauncherBasePath);
#else
    
    return relativePath;

#endif

}


inline bool SetProcessRunPath(const char * pszPath) {
    if (!pszPath) {
        return false;
    }

#ifdef _WIN32

    if (!SetCurrentDirectoryA(pszPath)) {
        char tempLog[512] = { 0 };
        sprintf_s(tempLog, "[%s] ����ִ��Ŀ¼���� !!!\n", pszPath);
        wchar_t tchar[512] = { 0 };
        MultiByteToWideChar(CP_ACP, 0, tempLog, -1, tchar, 256 * sizeof(TCHAR));
        MessageBox(nullptr, tchar, L"��ʾ!!!", 0);
        return false;
    }

#else

    bool ret = chdir(pszPath);
    if (ret != 0) {
        XERR(" ZoneServer::setRunBasePath chdir error [%s] \n", pszPath);
        return false;
    }

    char curCwd[256] = { 0 };
    getcwd(curCwd, 255);
    XLOG("Cur Linux Run Path : %s ", curCwd);

#endif

    printf("set run path [%s] success !!! \n", pszPath);
    return true;
}


