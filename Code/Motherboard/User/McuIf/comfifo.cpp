/**********************************************
            comfifo.c
            2014-11-8       zhangtaolei
***********************************************/

/**********************************************
            include
***********************************************/
#include "comfifo.h"

/**********************************************
		macro
***********************************************/
#ifndef DIM
#define DIM(x)		(sizeof(x)/sizeof(x[0]))
#endif

/**********************************************
            variable
***********************************************/

/**********************************************
            function
***********************************************/
void InitComFifo(ComFifo_t *pComFifo)
{
    pComFifo->bufSize = DIM(pComFifo->buf);
    pComFifo->writePos = 0;
    pComFifo->readPos = 0;
}

/* 返回成功写入的数据个数 */
int WriteComFifo(ComFifo_t *pComFifo, char *buf, int len)
{
    int writeCnt = 0;

    while ((pComFifo->writePos + 1)%(pComFifo->bufSize) != pComFifo->readPos)
    {
        pComFifo->buf[pComFifo->writePos] = buf[writeCnt];
        pComFifo->writePos = (pComFifo->writePos + 1)%pComFifo->bufSize;
        if (++writeCnt == len)
        {
            break;
        }
    }
    return writeCnt;
}

/* 返回读取成功的个数 */
int ReadComFifo(ComFifo_t *pComFifo, char *buf, int len)
{
    int readCnt = 0;

    while(pComFifo->readPos != pComFifo->writePos)
    {
        buf[readCnt] = pComFifo->buf[pComFifo->readPos];
        pComFifo->readPos = (pComFifo->readPos + 1)%pComFifo->bufSize;
        if (++readCnt == len)
        {
            break;
        }
    }
    
    return readCnt;
}

/* 返回读取成功的个数，该函数会从fifo中读取len个字节到buf，
但是实际fifo中的读指针不会发生变化，故名copy read */
int CopyReadComFifo(ComFifo_t *pComFifo, char *buf, int len)
{
    int readCnt = 0;
	int readPos = pComFifo->readPos;

    while(readPos != pComFifo->writePos)
    {
        buf[readCnt] = pComFifo->buf[readPos];
        readPos = (readPos + 1)%pComFifo->bufSize;
        if (++readCnt == len)
        {
            break;
        }
    }
    
    return readCnt;
}


/* 获取fifo中数据的长度 */
unsigned int GetFifoDataLen(ComFifo_t *pComFifo)
{
    if (pComFifo->writePos >= pComFifo->readPos)
    {
        return (pComFifo->writePos - pComFifo->readPos);
    }
    else
    {
        return (pComFifo->writePos + pComFifo->bufSize - pComFifo->readPos);
    }
}

unsigned int GetFifoFreeSpace(ComFifo_t *pComFifo)
{
  return 0;
}
