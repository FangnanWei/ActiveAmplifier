#include "Fifo.h"
#include "stm8s.h"
#include "stdlib.h"

Fifo g_UartFifo;
//Fifo *UartFifo;

void FifoInit()
{
	//UartFifo = &g_UartFifo;
    g_UartFifo.bufSize = DIM(g_UartFifo.buf);
    g_UartFifo.writePos = 0;
    g_UartFifo.readPos = 0;
}

int FifoAvailableBytes(Fifo *fifo) {
	int availableBytes;
	if (fifo->writePos < fifo->readPos) {
		availableBytes = fifo->bufSize - (fifo->readPos - fifo->writePos);
	}
	else {
		availableBytes = fifo->writePos - fifo->readPos;
	}
	//if (availableBytes < 0 || availableBytes > 128)
		//while(1);
	return availableBytes;
}

/* 返回成功写入的数据个数 */
int FifoWrite(Fifo *fifo, char *buf, int len)
{
    int writeCnt = 0;
    while ((fifo->writePos + 1)%(fifo->bufSize) != fifo->readPos)
    {
        fifo->buf[fifo->writePos] = buf[writeCnt];
        fifo->writePos = (fifo->writePos + 1)%fifo->bufSize;
        if (++writeCnt == len)
        {
            break;
        }
    }
    return writeCnt;
}

/* 返回读取成功的个数 */
int FifoRead(Fifo *fifo, char *buf, int len)
{
    int readCnt = 0;

    while(fifo->readPos != fifo->writePos)
    {
        buf[readCnt] = fifo->buf[fifo->readPos];
        fifo->readPos = (fifo->readPos + 1)%fifo->bufSize;
        if (++readCnt == len)
        {
            break;
        }
    }
    return readCnt;
}

