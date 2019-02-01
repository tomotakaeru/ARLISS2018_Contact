//EEPROM制御用関数
#ifndef _I2CEEPROM_H_
#define _I2CEEPROM_H_

#include<3052.h>
#include"I2CCommon.h"

extern int EEPROMAddress;
extern int Datas[];
extern unsigned int omega_datas[];//戻り値二つは出せないから、データ箱を用意
extern int motordata[];
extern unsigned int cnt_ch2;

int eepromwrite8byte(int byte1, int byte2, int byte3, int byte4, int byte5, int byte6, int byte7, int byte8);
int eepromrefresh(int upperByte);
int readalldata(int page);
int eepromwriteomegaandtime();
int eepromwritefirearm();
int eepromwritesuccessive();
int eepromWriteBulletsToFire(int bulletsToFire);

//4バイト書き込み用関数を定義,引数は書き込む内容（4バイト）,返り値は成功0,失敗1
int eepromwrite8byte(int byte1, int byte2, int byte3, int byte4 , int byte5, int byte6, int byte7, int byte8) {
	DI;//割り込み禁止

	int error;//エラー判定用変数
	if (byte1 < 0 || 255 < byte1) return 1;
	if (byte2 < 0 || 255 < byte2) return 1;
	if (byte3 < 0 || 255 < byte3) return 1;
	if (byte4 < 0 || 255 < byte4) return 1;
	if (byte5 < 0 || 255 < byte5) return 1;
	if (byte6 < 0 || 255 < byte6) return 1;
	if (byte7 < 0 || 255 < byte7) return 1;
	if (byte8 < 0 || 255 < byte8) return 1;

	//割り当てピンを設定
	P1.DDR = 0b11111111;//ポート1を使用
	P1.DR.BIT.B0 = 1;//ピン0はSCL
	P1.DR.BIT.B1 = 1;//ピン1はSDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//ピン2を電源にする,ここでEEPROMオン
	i2cwait();

	//1バイトの書き込みスタート
	P1.DR.BIT.B1 = 0;//startbit
	i2cinterval();
	P1.DR.BIT.B0 = 0;//clockstart
	i2cdelay();

	//コントロールバイト(書き)
	sendbyte(0b10100000);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//上位アドレスバイト
	sendbyte((EEPROMAddress >> 8) & 0b00011111);
	error = confirmACK();
	if (error)return 1;

	//下位アドレスバイト指定
	sendbyte(EEPROMAddress & 0xff);
	error = confirmACK();
	if (error)return 1;
	EEPROMAddress += 8;

	//1バイト目送信
	sendbyte(byte1);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//2バイト目送信
	sendbyte(byte2);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//3バイト目送信
	sendbyte(byte3);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//4バイト目送信
	sendbyte(byte4);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//5バイト目送信
	sendbyte(byte5);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//6バイト目送信
	sendbyte(byte6);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//7バイト目送信
	sendbyte(byte7);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//8バイト目送信
	sendbyte(byte8);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//ポート1を出力に
	i2cdelay();

	//書き込み終了
	stopbit();
	i2cwait();

	//EEPROMの電源0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}

//eepromのデータを256バイト分消去する,成功で0,失敗で1が返る
int eepromrefresh(int upperByte) {

	DI;//割り込み禁止

	int error;//エラー判定用変数

	//割り当てピンを設定
	P1.DDR = 0b11111111;//ポート1を使用
	P1.DR.BIT.B0 = 1;//ピン0はSCL
	P1.DR.BIT.B1 = 1;//ピン1はSDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//ピン2を電源にする,ここでEEPROMオン
	i2cwait();

	int roop;
	for (roop = 0; roop < 8; roop++)
	{
		//refreshスタート
		P1.DR.BIT.B1 = 0;//startbit
		i2cinterval();
		P1.DR.BIT.B0 = 0;//clockstart
		i2cdelay();

		//コントロールバイト(書き)
		sendbyte(0b10100000);
		error = confirmACK();
		if (error) return 1;
		i2cinterval();

		//上位アドレスバイトはupperByte
		sendbyte(upperByte & 0b00011111);
		error = confirmACK();
		if (error)return 1;

		//下位アドレスバイト指定
		sendbyte(32 * roop);
		error = confirmACK();
		if (error)return 1;
						  
		//データバイト(00000000)送信
		int i;
		for (i = 0; i < 32; i++) {
			sendbyte(0);
			error = confirmACK();
			if (error)return 1;
		}
		P1.DDR = 0b11111111;//ポート1を出力に
		i2cdelay();

		//書き込み終了
		stopbit();
		i2cwait();
	}

	EEPROMAddress = 0;//refreshによりaddressを0に戻す

	//EEPROMの電源0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}
//eepromのデータを64バイトよむ,エラーで1,成功で0が返る
int readalldata(int page) {

	DI;//割り込み禁止

	int error;//エラー判定用変数

			  //割り当てピンを設定
	P1.DDR = 0b11111111;//ポート1を使用
	P1.DR.BIT.B0 = 1;//ピン0はSCL
	P1.DR.BIT.B1 = 1;//ピン1はSDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//ピン2を電源にする,ここでEEPROMオン
	i2cwait();

	//コントロールデータ送信スタート
	P1.DR.BIT.B1 = 0;//startbit
	i2cinterval();
	P1.DR.BIT.B0 = 0;//clockstart
	i2cdelay();

	//コントロールバイト(書き)
	sendbyte(0b10100000);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//上位アドレスバイトは0x00
	sendbyte(0x00);
	error = confirmACK();
	if (error)return 1;

	//下位アドレスバイトも0x00
	sendbyte(64 * page);
	error = confirmACK();
	if (error)return 1;

	//startbit生成
	P1.DDR = 0b11111111;
	P1.DR.BIT.B0 = 1;
	P1.DR.BIT.B1 = 1;
	i2cinterval();
	P1.DR.BIT.B1 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	i2cdelay();

	//コントロールバイト(読み)
	sendbyte(0b10100001);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//1バイトずつ読みこみ,Datas[]に格納
	int i;
	for (i = 0; i < 63; i++)//64回目は特殊なので除外
	{
		Datas[i] = readbyte();
		sendACK(0);
	}
	Datas[63] = readbyte();
	sendACK(1);
	P1.DDR = 0b11111111;
	i2cdelay();

	//読み込み終了
	stopbit();
	i2cwait();

	//EEPROMの電源0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}

//角速度ログ取り用関数を定義,返り値は成功0,失敗1
int eepromwriteomegaandtime(){
	int error;
	error = eepromwrite8byte(
		(omega_datas[1] + 10) & 0xff,
		0,
		((omega_datas[0] >> 8) & 0xff),
		(omega_datas[0] & 0xff),
		((cnt_ch2 >> 8) & 0xff),
		(cnt_ch2 & 0xff),
		0,
		0
	);
	return error;
}

//モーターログ取り用関数を定義,返り値は成功0,失敗1
int eepromwritefirearm() {
	int error;
	error = eepromwrite8byte(
		20,
		0,
		0,
		0,
		0,
		motordata[0] & 0xff,
		motordata[1] & 0xff,
		motordata[2] & 0xff
	);
	return error;
}

//発射装置ログ取り用関数を定義,返り値は成功0,失敗1
int eepromwritesuccessive() {
	int error;
	error = eepromwrite8byte(
		20,
		0,
		0,
		0,
		0,
		0,
		0,
		motordata[2] & 0xff
	);
	return error;
}

//残り発射数予測ログ取り用関数を定義,返り値は成功0,失敗1
int eepromWriteBulletsToFire(int bulletsToFire) {
	int error;
	error = eepromwrite8byte(
		30,
		0,
		0,
		0,
		0,
		0,
		0,
		bulletsToFire & 0xff
	);
	return error;
}

#endif // !_I2CEEPROM_H_