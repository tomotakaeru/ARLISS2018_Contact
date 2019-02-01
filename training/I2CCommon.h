//I2C通信共通ヘッダファイル
#ifndef _I2CCOMMON_H_
#define _I2CCOMMON_H_

#include<3052.h>

extern int EEPROMAddress;
extern int Datas[];

void i2cwait(void);
void i2cinterval(void);
void i2cdelay(void);
void sendbyte(int byte);
void sendbit(int bit);
void clock(void);
void clock2(void);
int receivebit(void);
int confirmACK(void);
void stopbit(void);
int readbyte(void);
void sendACK(int ACK);

//時間待ち関数
void i2cwait(void) {
	volatile unsigned int i;
	volatile unsigned int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 0x1000; j++);
	}
}

void i2cinterval(void) {
	volatile unsigned int i;
	for (i = 0; i < 90; i++);
}

void i2cdelay(void) {
	volatile unsigned int i;
	for (i = 0; i < 50; i++);
}

//1バイト送る
void sendbyte(int byte) {
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();
	int i;
	for (i = 7; i >= 0; i--)
	{
		sendbit((byte >> i) & 1);
	}
}

//bitを送る
void sendbit(int bit) {
	P1.DR.BIT.B1 = bit;
	clock();
}

//Clockパルスを生成
void clock(void) {
	i2cdelay();
	P1.DR.BIT.B0 = 1;
	i2cdelay();
	P1.DR.BIT.B0 = 0;
	i2cdelay();
}

//Clockパルスを生成その2
void clock2(void) {
	P1.DR.BIT.B0 = 1;
	i2cdelay();
	P1.DR.BIT.B0 = 0;
	i2cdelay();
}

//P1を入力にしてbitを受け取る,P1が入力になるので注意
int receivebit(void) {
	P1.DDR = 0b11111101;
	i2cdelay();
	int i = P1.DR.BIT.B1;
	clock();
	return i;
}

//ACKを受け取る,返り値は0なら正常,P1が入力になるので注意
int confirmACK(void) {
	i2cdelay();
	int i = receivebit();
	return i;
}

//stopbitを送る
void stopbit(void) {
	P1.DR.BIT.B1 = 0;
	i2cdelay();
	P1.DR.BIT.B0 = 1;
	i2cdelay();
	P1.DR.BIT.B1 = 1;
	i2cdelay();
}

//1bit読み込む,int値を返す
int readbyte(void) {
	P1.DDR = 0b11111101;
	i2cdelay();
	int i = 0, sum = 0,bit = 0;
	for (i = 7; i >= 0; i--)
	{
		bit = P1.DR.BIT.B1;
		sum += (bit << i);
		clock2();
	}
	return sum;
}

//ACKを送る
void sendACK(int ACK) {
	P1.DDR = 0b11111111;
	i2cdelay();
	sendbit(ACK);
}
#endif