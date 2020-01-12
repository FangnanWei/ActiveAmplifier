#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "Common.h"
#include "DI_McuIfPublic.h"


typedef struct tagKvmThreadArg_t
{
	DI_McuIfHandle hdl;
	ThreadState_e threadState;
}KvmThreadArg_t;

void McuCb(DI_McuIfHandle hdl, DI_McuChangeType_e type, uint8_t channel, uint8_t *buf, uint16_t len)
{
	switch(type)
	{
		case MCU_CHANGE_UartReveice:
			{
				DebugPrint("receive %d from UART%d:%s", len, channel, (char*)buf);
			}
			break;

		case MCU_CHANGE_KeyBoardLedStatus:
			{
				DebugPrint("receive from MCU: buf [%d] \n", buf[0]);
				DebugPrint("receive from MCU: NumLock [%s], CapsLock [%s], ScrollLock [%s]",buf[0]&0x01 ? "ON" : "OFF", buf[0]&0x02 ? "ON" : "OFF", buf[0]&0x04 ? "ON" : "OFF");
			}
			break;
	}
}

void usage()
{
	printf("Usage:\n");
	printf("\t1 vs [COMn] -u|upgrade fileName\n");
	printf("\t2 vs [COMn] -v|version\n");
	printf("\t3 vs [COMn] -fv|firmwareVersion fileName\n");
	printf("\t4 vs [COMn] -re|readEeprom address len\n");
	printf("\t5 vs [COMn] -we|writeEeprom address [data in hex format ex: 02 ac 5d ...]\n");
	printf("\t6 vs [COMn] -wef|writeEepromFile address fileName\n");
	printf("\t7 vs [COMn] -rd|readDsp address len\n");
	printf("\t8 vs [COMn] -wd|writeDsp address [data in hex format ex: 02 ac 5d ...]\n");
	printf("\t9 vs [COMn] -ud|upgradeDsp fileName\n");
	printf("\t10 vs [COMn] -wde|writeDspExt address [data in hex format ex: 02 ac 5d ...]\n");
	printf("\t11 vs [COMn] -wsld|safeloadDsp address [data in hex format ex: 02 ac 5d ...]\n");
	printf("\t12 vs [COMn] -n|null (do nothing, wait report)\n");
}

void main (int argc, char **argv)
{
	int ret = 1;
	DI_McuIfHandle hdl = NULL;
	DI_McuPrintFnRegister(printf);

	if (2 >= argc)
	{
		usage();
		return 0;
	}

	if (0 == strcmp(argv[1], "-h") ||
		0 == strcmp(argv[1], "help"))
	{
		usage();
		return 0;
	}

	hdl = DI_McuIfInit(argv[1]);
	if (!hdl)
		return 0;

	DI_RegisterMcuChangeFn(hdl, McuCb);

	if (0 == strcmp(argv[2], "upgrade") ||
		0 == strcmp(argv[2], "-u"))
	{
		DI_McuFirmwareUpgrade(hdl, argv[3]);
		return 0;
	}
	else if (0 == strcmp(argv[2], "upgradeDsp") ||
		0 == strcmp(argv[2], "-ud"))
	{
		DI_DspTopoUpgrade(hdl, argv[3]);
		return 0;
	}
	else if (0 == strcmp(argv[2], "version") ||
		0 == strcmp(argv[2], "-v"))
	{
		char version[64] = {0};
		ret = DI_McuGetSoftwareVersion(hdl, version, sizeof(version));
		DebugPrint("ret %d, version: %s", ret, version);
		return 0;
	}
	else if (0 == strcmp(argv[2], "firmwareVersion") ||
		0 == strcmp(argv[2], "-fv"))
	{
		char version[16] = {0};
		ret = DI_McuGetFirmwareVersion(argv[3], version, 16);
		DebugPrint("ret %d version %s", ret, version);
		return 0;
	}
	else if (0 == strcmp(argv[2], "readEeprom") ||
		0 == strcmp(argv[2], "-re"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;

		address = atoi(argv[3]);
		len = atoi(argv[4]);
		printf("read EEPROM %d size %d\n", address, len);
		DI_McuReadEeprom(hdl, buf, address, len);
		HexPrint(buf, len, 16);
		return 0;
	}
	else if (0 == strcmp(argv[2], "writeEeprom") ||
		0 == strcmp(argv[2], "-we"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;
		int i = 0;

		address = atoi(argv[3]);
		len = argc - 4;
		printf("write EEPROM %d size %d\n", address, len);

		for (i = 0; i < len; i++)
		{
			sscanf(argv[4+i], "%02x", &buf[i]);
		}
		
		DI_McuWriteE2prom(hdl, buf, address, len);
		return 0;
	}
	else if (0 == strcmp(argv[2], "writeEepromFile") ||
		0 == strcmp(argv[2], "-wef"))
	{
		uint16_t address = 0;
		address = atoi(argv[3]);
		DI_McuWriteFileToE2prom(hdl, address, argv[4]);
		return 0;
	}
	else if (0 == strcmp(argv[2], "readDsp") ||
		0 == strcmp(argv[2], "-rd"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;

		address = atoi(argv[3]);
		len = atoi(argv[4]);
		printf("read DSP %d size %d\n", address, len);
		DI_McuReadDsp(hdl, buf, address, len);
		HexPrint(buf, len, 16);
		return 0;
	}
	else if (0 == strcmp(argv[2], "writeDsp") ||
		0 == strcmp(argv[2], "-wd"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;
		int i = 0;

		address = atoi(argv[3]);
		len = argc - 4;
		printf("write DSP %d size %d\n", address, len);

		for (i = 0; i < len; i++)
		{
			sscanf(argv[4+i], "%02x", &buf[i]);
		}
		
		DI_McuWriteDsp(hdl, buf, address, len);
		return 0;
	}
	else if (0 == strcmp(argv[2], "writeDspExt") ||
		0 == strcmp(argv[2], "-wde"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;
		int i = 0;

		address = atoi(argv[3]);
		len = argc - 4;
		printf("write DSP %d size %d\n", address, len);

		for (i = 0; i < len; i++)
		{
			sscanf(argv[4+i], "%02x", &buf[i]);
		}
		
		DI_McuWriteDspExt(hdl, buf, address, len);
		return 0;
	}
	else if (0 == strcmp(argv[2], "writeSafeloadDspExt") ||
		0 == strcmp(argv[2], "-wsde"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;
		int i = 0;

		address = atoi(argv[3]);
		len = argc - 4;
		printf("write DSP %d size %d\n", address, len);

		for (i = 0; i < len; i++)
		{
			sscanf(argv[4+i], "%02x", &buf[i]);
		}
		
		DI_McuSafeloadDspExt(hdl, buf, address, len);
		return 0;
	}
	else if (0 == strcmp(argv[2], "safeloadDsp") ||
		0 == strcmp(argv[2], "-wsld"))
	{
		uint8_t buf[256] = {0};
		uint16_t address = 0;
		uint16_t len = 0;
		int i = 0;

		address = atoi(argv[3]);
		len = argc - 4;
		printf("safeloadDsp DSP %d size %d\n", address, len);

		for (i = 0; i < len; i++)
		{
			sscanf(argv[4+i], "%02x", &buf[i]);
		}
		
		DI_McuSafeloadDsp(hdl, buf, address, len);
		return 0;
	}
	else if (0 == strcmp(argv[2], "null") ||
		0 == strcmp(argv[2], "-n"))
	{
		while(1)
		{
			Sleep(1);
		}
	}
	
	DI_McuIfDeInit(hdl);
}


