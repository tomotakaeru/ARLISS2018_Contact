//通信用ヘッダファイル
#ifndef _EEPROMINSTRUCTIONS_H_
#define _EEPROMINSTRUCTIONS_H_

#include<3052.h>
#include<string.h>
#include"I2CEEPROM.h"
#include"I2CCommon.h"
#include"stepping_motor.h"

#define instructionlength 256 //指令は256文字まで(\0含む)

extern int Datas[];//dataを読み出すときの入れ物

int readinstruction(void);
void int_irq0(void);
void ConsoleWrite();
void ConsoleWrite4Byte(int i);

//instructionを読み出す,返り値はRead:1,Refresh:2,それ以外:0
int readinstruction(void) {

	EI;

	//receive instruction
	char instruction[instructionlength];
	SCI1_IN_STRING(instruction, instructionlength);
	if (strcmp(instruction, "read") == 0) {
		SCI1_PRINTF("which page\? 0-3\n");
		SCI1_IN_STRING(instruction, instructionlength);
		if (strcmp(instruction, "0") == 0)
		{
			SCI1_PRINTF("start reading\n");
			return 10;
		}
		else if (strcmp(instruction, "1") == 0)
		{
			SCI1_PRINTF("start reading\n");
			return 11;
		}
		else if (strcmp(instruction, "2") == 0)
		{
			SCI1_PRINTF("start reading\n");
			return 12;
		}
		else if (strcmp(instruction, "3") == 0)
		{
			SCI1_PRINTF("start reading\n");
			return 13;
		}
		else if (strcmp(instruction, "all") == 0) {
			SCI1_PRINTF("start reading\n");
			return 9;
		}
		else{
		SCI1_PRINTF("error\n");
		return 0;
		}
	}
	else if (strcmp(instruction, "refresh") == 0) {
		SCI1_PRINTF("refresh EEPROM\? continue:\"yes\"\n");
		SCI1_IN_STRING(instruction, instructionlength);
		if (strcmp(instruction, "yes") == 0) {
			SCI1_PRINTF("start refreshing\n");
			return 2;
		}
		else {
			SCI1_PRINTF("quit refreshing\n");
			return 0;
		}
	}
	else if (strcmp(instruction, "write") == 0) {
		return 100;
	}
	else {
		SCI1_PRINTF("error\n");
		return 0;
	}
}

//IRQ0端子の立下りで割り込み開始,以降電源を切るまでずっとループ
/*
main関数内に
SYSCR.BIT.UE = 1;
INTC.ISCR.BIT.IRQ0SC = 1;//入力立下りで割り込み
INTC.IPRA.BIT._IRQ0 = 1;//priority level 1
INTC.ISR.BIT.IRQ0F = 0;//割り込みステータスクリア
INTC.IER.BIT.IRQ0E = 1;//interrupt enable(IRQ0)
の記述が必要。
*/
void int_irq0(void) {
	DI;
	PB.DDR = 0xff;
	PB.DR.BYTE = 0xff;
	INTC.IER.BIT.IRQ0E = 0;//interrupt disable(IRQ0)
	INTC.IER.BIT.IRQ1E = 0;//interrupt disable(IRQ1)
	PB.DR.BIT.B0 = 0;
	tuna_set();
	SCI1_PRINTF("Ready\n");
	EI;
	while (1) {
		short error = 0;
		int instructiontype = readinstruction();
		PB.DR.BIT.B1 = 0;
		switch (instructiontype)
		{
		case 9:
			SCI1_PRINTF("reading\n");
			error = readalldata(0);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			i2cwait();
			error = readalldata(1);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			i2cwait();
			error = readalldata(2);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			i2cwait();
			error = readalldata(3);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			break;
		case 10:
			SCI1_PRINTF("reading\n");
			error = readalldata(0);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			break;
		case 11:
			SCI1_PRINTF("reading\n");
			error = readalldata(1);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			break;
		case 12:
			SCI1_PRINTF("reading\n");
			error = readalldata(2);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			break;
		case 13:
			SCI1_PRINTF("reading\n");
			error = readalldata(3);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				ConsoleWrite();
			}
			break;
		case 2:
			SCI1_PRINTF("erasing\n");
			error = eepromrefresh(0);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
				break;
			}
			error = eepromrefresh(1);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
				break;
			}
			error = eepromrefresh(2);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
				break;
			}
			error = eepromrefresh(3);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
				break;
			}
			error = eepromrefresh(4);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
				break;
			}
			error = eepromrefresh(5);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				SCI1_PRINTF("Done refreshing\n");
			}
			break;
		case 100:
			error = eepromwrite8byte(1, 2, 3, 4, 5, 6, 7, 8);
			if (error)
			{
				SCI1_PRINTF("Error!\n");
			}
			else {
				SCI1_PRINTF("OK\n");
			}
			break;
		default:
			break;
		}
		i2cwait();
	}
}

void ConsoleWrite() {
	ConsoleWrite4Byte(0);
	ConsoleWrite4Byte(1);
	ConsoleWrite4Byte(2);
	ConsoleWrite4Byte(3);
	ConsoleWrite4Byte(4);
	ConsoleWrite4Byte(5);
	ConsoleWrite4Byte(6);
	ConsoleWrite4Byte(7);
}

void ConsoleWrite4Byte(int i) {
	SCI1_PRINTF("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", Datas[8 * i], Datas[8 * i + 1], Datas[8 * i + 2], Datas[8 * i + 3], Datas[8 * i + 4], Datas[8 * i + 5], Datas[8 * i + 6], Datas[8 * i + 7]);
}

#endif // !_EEPROMINSTRUCTIONS_H_