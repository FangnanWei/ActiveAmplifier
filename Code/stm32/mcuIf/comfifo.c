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

/* ���سɹ�д������ݸ��� */
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

/* ���ض�ȡ�ɹ��ĸ��� */
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

/* ���ض�ȡ�ɹ��ĸ������ú������fifo�ж�ȡlen���ֽڵ�buf��
����ʵ��fifo�еĶ�ָ�벻�ᷢ���仯������copy read */
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


/* ��ȡfifo�����ݵĳ��� */
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
