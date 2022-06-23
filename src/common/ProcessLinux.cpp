#include "ProcessLinux.h"

#ifndef _WIN32

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h>

#define MAXLINE 2048

char **environ = nullptr;

static char **_main_Argv = NULL;    /* pointer to argument vector */
static char *_main_LastArgv = NULL;    /* end of argv */

void setproctitle_init(int argc, char **argv, char **envp) {
    int i = 0;
    for (i = 0; envp[i] != NULL; i++) {// calc envp num
        continue;
    }

    environ = (char **)malloc(sizeof(char *) * (i + 1)); // malloc envp pointer

    for (i = 0; envp[i] != NULL; i++) {
        environ[i] = (char*)malloc(sizeof(char) * strlen(envp[i]));
        strcpy(environ[i], envp[i]);
    }
    environ[i] = NULL;

    _main_Argv = argv;
    if (i > 0)
        _main_LastArgv = envp[i - 1] + strlen(envp[i - 1]);
    else
        _main_LastArgv = argv[argc - 1] + strlen(argv[argc - 1]);
}

void setproctitle(const char *fmt, ...) {
    char *p = nullptr;
    int i = 0;
    char buf[MAXLINE] = {0};

    va_list ap;
    p = buf;

    va_start(ap, fmt);
    vsprintf(p, fmt, ap);
    va_end(ap);

    i = strlen(buf);

    if (i > _main_LastArgv - _main_Argv[0] - 2) {
        i = _main_LastArgv - _main_Argv[0] - 2;
        buf[i] = '\0';
    }
    //修改argv[0]
    (void)strcpy(_main_Argv[0], buf);

    p = &_main_Argv[0][i];
    while (p < _main_LastArgv)
        *p++ = '\0';
    _main_Argv[1] = NULL;

    //调用prctl
    prctl(PR_SET_NAME, buf);
}

int EditProcessName(int argc, char *argv[], const char *pProcessName) {
    char argv_buf[MAXLINE] = { 0 }; // save argv paramters
    int i = 0;

    for (i = 1; i < argc; i++) {
        strcat(argv_buf, argv[i]);
        strcat(argv_buf, " ");
    }

    // 修改argv[0]所指向的内存空间的内容
    setproctitle_init(argc, argv, environ);

    //调用prctl修改进程名
    setproctitle(pProcessName);

    for (i = 0; environ[i] != NULL; i++) {
        free(environ[i]);
    }

    return 0;
}

#endif