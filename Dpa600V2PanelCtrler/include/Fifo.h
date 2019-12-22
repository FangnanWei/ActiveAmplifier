#ifndef _Fifo_H_
#define _Fifo_H_

#define FifoMaxSize    256

typedef struct Fifo_t
{
    int bufSize;
    char buf[FifoMaxSize];     /* FIFO的缓存区 */
    int writePos;                   /* 当前写入数据的下标 */
    int readPos;                    /* 当前被读取数据的下标 */
}Fifo;

//extern Fifo *UartFifo;
extern Fifo g_UartFifo;
int FifoRead(Fifo *fifo, char *buf, int len);
int FifoWrite(Fifo *fifo, char *buf, int len);
void FifoInit();
int FifoAvailableBytes(Fifo *fifo);
#define DIM(s)  (sizeof(s)/sizeof(s[0]))

#endif

