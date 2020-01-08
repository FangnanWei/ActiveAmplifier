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
SIGABRT		�ɵ���abort�������������̷������˳�
SIGALRM		��alarm�������õ�timer��ʱ��setitimer�������õ�interval timer��ʱ
SIGBUS			ĳ���ض���Ӳ���쳣��ͨ�����ڴ��������
SIGCANCEL		��Solaris Thread Library�ڲ�ʹ�ã�ͨ������ʹ��
SIGCHLD		����Terminate��Stop��ʱ��SIGCHLD�ᷢ�͸����ĸ����̡�ȱʡ����¸�Signal�ᱻ����
SIGCONT		����stop�Ľ��ָ̻����е�ʱ���Զ�����
SIGEMT			��ʵ����ص�Ӳ���쳣
SIGFPE			��ѧ��ص��쳣���类0��������������ȵ�
SIGFREEZE		Solarisר�ã�Hiberate����Suspendedʱ����
SIGHUP			���͸�����Terminal��Controlling Process����terminal��disconnectʱ����
SIGILL			�Ƿ�ָ���쳣
SIGINFO			BSD signal����Status Key������ͨ����CTRL+T�����͸�����Foreground Group�Ľ���
SIGINT			��Interrupt Key������ͨ����CTRL+C����DELETE�����͸�����ForeGround Group�Ľ���
SIGIO			�첽IO�¼�
SIGIOT			ʵ����ص�Ӳ���쳣��һ���ӦSIGABRT
SIGKILL			�޷�����ͺ��ԡ���ֹĳ������
SIGLWP			��Solaris Thread Libray�ڲ�ʹ��
SIGPIPE			��reader��ֹ֮��дPipe��ʱ����
SIGPOLL			��ĳ���¼����͸�Pollable Device��ʱ����
SIGPROF			Setitimerָ����Profiling Interval Timer������
SIGPWR			��ϵͳ��ء���UPS��ء�
SIGQUIT			����Quit Key��ʱ��CTRL+\�����͸�����Foreground Group�Ľ���
SIGSEGV		�Ƿ��ڴ����
SIGSTKFLT		Linuxר�ã���ѧЭ��������ջ�쳣
SIGSTOP		��ֹ���̡��޷�����ͺ��ԡ�
SIGSYS			�Ƿ�ϵͳ����
SIGTERM		������ֹ���̣�kill����ȱʡ����
SIGTHAW		Solarisר�ã���Suspend�ָ�ʱ����
SIGTRAP			ʵ����ص�Ӳ���쳣��һ���ǵ����쳣
SIGTSTP			Suspend Key��һ����Ctrl+Z�����͸�����Foreground Group�Ľ���
SIGTTIN			��Background Group�Ľ��̳��Զ�ȡTerminal��ʱ����
SIGTTOU		��Background Group�Ľ��̳���дTerminal��ʱ����
SIGURG			��out-of-band data���յ�ʱ����ܷ���
SIGUSR1		�û��Զ���signal 1
SIGUSR2		�û��Զ���signal 2
SIGVTALRM		setitimer�������õ�Virtual Interval Timer��ʱ��ʱ��
SIGWAITING		Solaris Thread Library�ڲ�ʵ��ר��
SIGWINCH		��Terminal�Ĵ��ڴ�С�ı��ʱ�򣬷��͸�Foreground Group�����н���
SIGXCPU		��CPUʱ�����Ƴ�ʱ��ʱ��
SIGXFSZ		���̳����ļ���С����
SIGXRES		Solarisר�ã����̳�����Դ���Ƶ�ʱ����

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



