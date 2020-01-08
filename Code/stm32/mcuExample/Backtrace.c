/**********************************************
					Backtrace.c
***********************************************/

/**********************************************
					include
***********************************************/
#include <execinfo.h>
#include <stdio.h>
#include <mcheck.h>
#include <stdlib.h>
#include <signal.h>

/**********************************************
					macro
***********************************************/

/**********************************************
					typedef
***********************************************/

/**********************************************
					variables
***********************************************/

/**********************************************
					functions
***********************************************/
void exceptionHdl(int signum)
{
	int i = 0;
	#define SIZE 100
	void *buffer[SIZE];
    int nptrs = 0;
    char **strings = NULL;

    printf("exit with signal %d\n", signum);
//	SysDebugLog("exit with signal %d", signum);
	nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
//	SysDebugLog("backtrace() returned %d addresses", nptrs);

#if 0
    backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO);
#else
    strings = (char **)backtrace_symbols(buffer, nptrs);
    for (i = 0; i < nptrs; i++)
    {
        printf("%d %s\n", i, strings[i]);
    }
    free(strings);
#endif

    exit(-1);

}


void exceptionNormalHdl(int signum)
{
    exit(0);
}

void EnableBackTrace(void)
{
    signal(SIGINT,  exceptionHdl);
    signal(SIGQUIT,  exceptionHdl);
    signal(SIGILL,  exceptionHdl);
    signal(SIGTRAP,  exceptionHdl);
//    signal(SIGABRT,  exceptionHdl);
    signal(SIGIOT,  exceptionHdl);
    signal(SIGBUS,  exceptionHdl);
    signal(SIGFPE,  exceptionHdl);
    signal(SIGKILL,  exceptionHdl);
    signal(SIGUSR1,  exceptionHdl);
    signal(SIGSEGV,  exceptionHdl);
    signal(SIGUSR2,  exceptionHdl);
//    signal(SIGPIPE,  exceptionHdl);
    signal(SIGALRM,  exceptionHdl);
    signal(SIGTERM,  exceptionHdl);
    signal(SIGSTKFLT,  exceptionHdl);
//    signal(SIGCLD,  exceptionHdl);
//    signal(SIGCHLD,  exceptionHdl);
    signal(SIGCONT,  exceptionHdl);
    signal(SIGSTOP,  exceptionHdl);
    signal(SIGTSTP,  exceptionHdl);
    signal(SIGTTIN,  exceptionHdl);
    signal(SIGTTOU,  exceptionHdl);
    signal(SIGURG,  exceptionHdl);
    signal(SIGXCPU,  exceptionHdl);
    signal(SIGXFSZ,  exceptionHdl);
    signal(SIGVTALRM,  exceptionHdl);
    signal(SIGPROF,  exceptionHdl);
    signal(SIGWINCH,  exceptionHdl);
    signal(SIGPOLL,  exceptionHdl);
    signal(SIGIO,  exceptionHdl);
    signal(SIGPWR,  exceptionHdl);
    signal(SIGSYS,  exceptionHdl);
}

void EnableNormalSignal(void)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT,  exceptionHdl);
    signal(SIGQUIT,  exceptionHdl);
    signal(SIGILL,  exceptionHdl);
    signal(SIGTRAP,  exceptionHdl);
//    signal(SIGABRT,  exceptionHdl);
    signal(SIGIOT,  exceptionHdl);
    signal(SIGBUS,  exceptionHdl);
    signal(SIGFPE,  exceptionHdl);
    signal(SIGKILL,  exceptionHdl);
    signal(SIGUSR1,  exceptionHdl);
    signal(SIGSEGV,  exceptionHdl);
    signal(SIGUSR2,  exceptionHdl);
//    signal(SIGPIPE,  exceptionHdl);
    signal(SIGALRM,  exceptionHdl);
    signal(SIGTERM,  exceptionHdl);
    signal(SIGSTKFLT,  exceptionHdl);
//    signal(SIGCLD,  exceptionHdl);
//    signal(SIGCHLD,  exceptionHdl);
    signal(SIGCONT,  exceptionHdl);
    signal(SIGSTOP,  exceptionHdl);
    signal(SIGTSTP,  exceptionHdl);
    signal(SIGTTIN,  exceptionHdl);
    signal(SIGTTOU,  exceptionHdl);
    signal(SIGURG,  exceptionHdl);
    signal(SIGXCPU,  exceptionHdl);
    signal(SIGXFSZ,  exceptionHdl);
    signal(SIGVTALRM,  exceptionHdl);
    signal(SIGPROF,  exceptionHdl);
    signal(SIGWINCH,  exceptionHdl);
    signal(SIGPOLL,  exceptionHdl);
    signal(SIGIO,  exceptionHdl);
    signal(SIGPWR,  exceptionHdl);
    signal(SIGSYS,  exceptionHdl);
}

void SetMTraceEnv(const char *pFileName)
{
    setenv("MALLOC_TRACE", pFileName, 1);
}

void StartMTrace(void)
{
    mtrace();
}

void StopMTrace(void)
{
    muntrace();
}


/*
SIGABRT		由调用abort函数产生，进程非正常退出
SIGALRM		用alarm函数设置的timer超时或setitimer函数设置的interval timer超时
SIGBUS			某种特定的硬件异常，通常由内存访问引起
SIGCANCEL		由Solaris Thread Library内部使用，通常不会使用
SIGCHLD		进程Terminate或Stop的时候，SIGCHLD会发送给它的父进程。缺省情况下该Signal会被忽略
SIGCONT		当被stop的进程恢复运行的时候，自动发送
SIGEMT			和实现相关的硬件异常
SIGFPE			数学相关的异常，如被0除，浮点溢出，等等
SIGFREEZE		Solaris专用，Hiberate或者Suspended时候发送
SIGHUP			发送给具有Terminal的Controlling Process，当terminal被disconnect时候发送
SIGILL			非法指令异常
SIGINFO			BSD signal。由Status Key产生，通常是CTRL+T。发送给所有Foreground Group的进程
SIGINT			由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
SIGIO			异步IO事件
SIGIOT			实现相关的硬件异常，一般对应SIGABRT
SIGKILL			无法处理和忽略。中止某个进程
SIGLWP			由Solaris Thread Libray内部使用
SIGPIPE			在reader中止之后写Pipe的时候发送
SIGPOLL			当某个事件发送给Pollable Device的时候发送
SIGPROF			Setitimer指定的Profiling Interval Timer所产生
SIGPWR			和系统相关。和UPS相关。
SIGQUIT			输入Quit Key的时候（CTRL+\）发送给所有Foreground Group的进程
SIGSEGV		非法内存访问
SIGSTKFLT		Linux专用，数学协处理器的栈异常
SIGSTOP		中止进程。无法处理和忽略。
SIGSYS			非法系统调用
SIGTERM		请求中止进程，kill命令缺省发送
SIGTHAW		Solaris专用，从Suspend恢复时候发送
SIGTRAP			实现相关的硬件异常。一般是调试异常
SIGTSTP			Suspend Key，一般是Ctrl+Z。发送给所有Foreground Group的进程
SIGTTIN			当Background Group的进程尝试读取Terminal的时候发送
SIGTTOU		当Background Group的进程尝试写Terminal的时候发送
SIGURG			当out-of-band data接收的时候可能发送
SIGUSR1		用户自定义signal 1
SIGUSR2		用户自定义signal 2
SIGVTALRM		setitimer函数设置的Virtual Interval Timer超时的时候
SIGWAITING		Solaris Thread Library内部实现专用
SIGWINCH		当Terminal的窗口大小改变的时候，发送给Foreground Group的所有进程
SIGXCPU		当CPU时间限制超时的时候
SIGXFSZ		进程超过文件大小限制
SIGXRES		Solaris专用，进程超过资源限制的时候发送

#define SIGHUP           1
#define SIGINT           2
#define SIGQUIT          3
#define SIGILL           4
#define SIGTRAP          5
#define SIGABRT          6
#define SIGIOT           6
#define SIGBUS           7
#define SIGFPE           8
#define SIGKILL          9
#define SIGUSR1         10
#define SIGSEGV         11
#define SIGUSR2         12
#define SIGPIPE         13
#define SIGALRM         14
#define SIGTERM         15
#define SIGSTKFLT       16
#define SIGIOT           6
#define SIGBUS           7
#define SIGFPE           8
#define SIGKILL          9
#define SIGUSR1         10
#define SIGSEGV         11
#define SIGUSR2         12
#define SIGPIPE         13
#define SIGALRM         14
#define SIGTERM         15
#define SIGSTKFLT       16
#define SIGCHLD         17
#define SIGCONT         18
#define SIGSTOP         19
#define SIGTSTP         20
#define SIGTTIN         21
#define SIGTTOU         22
#define SIGURG          23
#define SIGXCPU         24
#define SIGXFSZ         25
#define SIGVTALRM       26
#define SIGPROF         27
#define SIGWINCH        28
#define SIGIO           29
#define SIGPOLL         SIGIO
#define SIGLOST         29
#define SIGPWR          30
#define SIGSYS          31
#define SIGUNUSED       31
*/



