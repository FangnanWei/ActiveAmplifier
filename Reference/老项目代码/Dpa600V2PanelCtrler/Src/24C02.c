#include "stm8s.h"
#include "24C02.h"
#include "I2c.h"
#include "string.h"
#include "Delay.h"
#if 0
int Read24c02(uint16_t addr, uint8_t *buf,uint16_t num)
{
    if(num==0)
    	return 1;

    return I2C_Read(I2cAddr_E2prom, addr, buf, num);
}

int Write24c02(uint16_t addr,uint8_t *buf,uint16_t len)
{
	uint16_t nextPageBase, lastPageBase, firstPageRestSize;
	int16_t restLen;
	uint8_t *dataBuf = buf;
	if (len == 0)
		return -1;
	nextPageBase = (addr / At24c02WritePageSize + 1) * At24c02WritePageSize;
	firstPageRestSize = nextPageBase - addr;
	restLen = len - firstPageRestSize;
	I2C_Write(I2cAddr_E2prom, addr, dataBuf, restLen > 0 ? firstPageRestSize : len);
	if (restLen > 0) {
		lastPageBase = (addr + len - 1) / At24c02WritePageSize * At24c02WritePageSize;
		dataBuf += firstPageRestSize;
		for (; nextPageBase < lastPageBase; nextPageBase += At24c02WritePageSize) {
			I2C_Write(I2cAddr_E2prom, nextPageBase, dataBuf, At24c02WritePageSize);
			restLen -= At24c02WritePageSize;
			dataBuf += At24c02WritePageSize;
            DelayNop(1);
		}
        if (restLen > 0) {
          I2C_Write(I2cAddr_E2prom, lastPageBase, dataBuf, restLen);
          Delay(1);
        }
	}
	return 0;
}
#endif
#if 0
uint8_t data[512] = {0};
void test24c02(void)
{
    int i = 0, offset;
    for(i = 0; i < 512; i++)
    {
        data[i] = i % 256;
    }
    Write24c02(0, data, 512);
	memset(data, 0, 512);
    Delay(100);
	//for (i = 0; i < 64; i++) {
	//	offset = i * 8;
	//	Read24c02(offset, data + offset, 8);
	//}
    Read24c02(0, data, 512);
    data[511] = 0;
}
#endif

