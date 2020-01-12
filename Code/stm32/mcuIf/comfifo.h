/**********************************************
            comfifo.h
            2014-11-8       zhangtaolei
***********************************************/

#ifndef _COMFIFO_H_
#define _COMFIFO_H_

/**********************************************
            include
***********************************************/

/**********************************************
            macro
***********************************************/
#define COMFIFO_MAX_SIZE    8*1024
/**********************************************
            typedef
***********************************************/
typedef struct tagComFifo_t
{
    int bufSize;
    char buf[COMFIFO_MAX_SIZE];     /* FIFO的缓存区 */
    int writePos;                   /* 当前写入数据的下标 */
    int readPos;                    /* 当前被读取数据的下标 */
}ComFifo_t;

/**********************************************
            variables
**********************************************/

/**********************************************
            function
***********************************************/
int ReadComFifo(ComFifo_t *pComFifo, char *buf, int len);
int CopyReadComFifo(ComFifo_t *pComFifo, char *buf, int len);
int WriteComFifo(ComFifo_t *pComFifo, char *buf, int len);
void InitComFifo(ComFifo_t *pComFifo);
unsigned int GetFifoDataLen(ComFifo_t *pComFifo);
#endif

