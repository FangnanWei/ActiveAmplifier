#ifndef _Fifo_H_
#define _Fifo_H_

#define FifoMaxSize    256

typedef struct Fifo_t
{
    int bufSize;
    char buf[FifoMaxSize];     /* FIFO�Ļ����� */
    int writePos;                   /* ��ǰд�����ݵ��±� */
    int readPos;                    /* ��ǰ����ȡ���ݵ��±� */
}Fifo;

//extern Fifo *UartFifo;
extern Fifo g_UartFifo;
int FifoRead(Fifo *fifo, char *buf, int len);
int FifoWrite(Fifo *fifo, char *buf, int len);
void FifoInit();
int FifoAvailableBytes(Fifo *fifo);
#define DIM(s)  (sizeof(s)/sizeof(s[0]))

#endif

