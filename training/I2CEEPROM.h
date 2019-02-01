//EEPROM����p�֐�
#ifndef _I2CEEPROM_H_
#define _I2CEEPROM_H_

#include<3052.h>
#include"I2CCommon.h"

extern int EEPROMAddress;
extern int Datas[];
extern unsigned int omega_datas[];//�߂�l��͏o���Ȃ�����A�f�[�^����p��
extern int motordata[];
extern unsigned int cnt_ch2;

int eepromwrite8byte(int byte1, int byte2, int byte3, int byte4, int byte5, int byte6, int byte7, int byte8);
int eepromrefresh(int upperByte);
int readalldata(int page);
int eepromwriteomegaandtime();
int eepromwritefirearm();
int eepromwritesuccessive();
int eepromWriteBulletsToFire(int bulletsToFire);

//4�o�C�g�������ݗp�֐����`,�����͏������ޓ��e�i4�o�C�g�j,�Ԃ�l�͐���0,���s1
int eepromwrite8byte(int byte1, int byte2, int byte3, int byte4 , int byte5, int byte6, int byte7, int byte8) {
	DI;//���荞�݋֎~

	int error;//�G���[����p�ϐ�
	if (byte1 < 0 || 255 < byte1) return 1;
	if (byte2 < 0 || 255 < byte2) return 1;
	if (byte3 < 0 || 255 < byte3) return 1;
	if (byte4 < 0 || 255 < byte4) return 1;
	if (byte5 < 0 || 255 < byte5) return 1;
	if (byte6 < 0 || 255 < byte6) return 1;
	if (byte7 < 0 || 255 < byte7) return 1;
	if (byte8 < 0 || 255 < byte8) return 1;

	//���蓖�ăs����ݒ�
	P1.DDR = 0b11111111;//�|�[�g1���g�p
	P1.DR.BIT.B0 = 1;//�s��0��SCL
	P1.DR.BIT.B1 = 1;//�s��1��SDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//�s��2��d���ɂ���,������EEPROM�I��
	i2cwait();

	//1�o�C�g�̏������݃X�^�[�g
	P1.DR.BIT.B1 = 0;//startbit
	i2cinterval();
	P1.DR.BIT.B0 = 0;//clockstart
	i2cdelay();

	//�R���g���[���o�C�g(����)
	sendbyte(0b10100000);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//��ʃA�h���X�o�C�g
	sendbyte((EEPROMAddress >> 8) & 0b00011111);
	error = confirmACK();
	if (error)return 1;

	//���ʃA�h���X�o�C�g�w��
	sendbyte(EEPROMAddress & 0xff);
	error = confirmACK();
	if (error)return 1;
	EEPROMAddress += 8;

	//1�o�C�g�ڑ��M
	sendbyte(byte1);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//2�o�C�g�ڑ��M
	sendbyte(byte2);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//3�o�C�g�ڑ��M
	sendbyte(byte3);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//4�o�C�g�ڑ��M
	sendbyte(byte4);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//5�o�C�g�ڑ��M
	sendbyte(byte5);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//6�o�C�g�ڑ��M
	sendbyte(byte6);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//7�o�C�g�ڑ��M
	sendbyte(byte7);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//8�o�C�g�ڑ��M
	sendbyte(byte8);
	error = confirmACK();
	if (error)return 1;
	P1.DDR = 0b11111111;//�|�[�g1���o�͂�
	i2cdelay();

	//�������ݏI��
	stopbit();
	i2cwait();

	//EEPROM�̓d��0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}

//eeprom�̃f�[�^��256�o�C�g����������,������0,���s��1���Ԃ�
int eepromrefresh(int upperByte) {

	DI;//���荞�݋֎~

	int error;//�G���[����p�ϐ�

	//���蓖�ăs����ݒ�
	P1.DDR = 0b11111111;//�|�[�g1���g�p
	P1.DR.BIT.B0 = 1;//�s��0��SCL
	P1.DR.BIT.B1 = 1;//�s��1��SDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//�s��2��d���ɂ���,������EEPROM�I��
	i2cwait();

	int roop;
	for (roop = 0; roop < 8; roop++)
	{
		//refresh�X�^�[�g
		P1.DR.BIT.B1 = 0;//startbit
		i2cinterval();
		P1.DR.BIT.B0 = 0;//clockstart
		i2cdelay();

		//�R���g���[���o�C�g(����)
		sendbyte(0b10100000);
		error = confirmACK();
		if (error) return 1;
		i2cinterval();

		//��ʃA�h���X�o�C�g��upperByte
		sendbyte(upperByte & 0b00011111);
		error = confirmACK();
		if (error)return 1;

		//���ʃA�h���X�o�C�g�w��
		sendbyte(32 * roop);
		error = confirmACK();
		if (error)return 1;
						  
		//�f�[�^�o�C�g(00000000)���M
		int i;
		for (i = 0; i < 32; i++) {
			sendbyte(0);
			error = confirmACK();
			if (error)return 1;
		}
		P1.DDR = 0b11111111;//�|�[�g1���o�͂�
		i2cdelay();

		//�������ݏI��
		stopbit();
		i2cwait();
	}

	EEPROMAddress = 0;//refresh�ɂ��address��0�ɖ߂�

	//EEPROM�̓d��0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}
//eeprom�̃f�[�^��64�o�C�g���,�G���[��1,������0���Ԃ�
int readalldata(int page) {

	DI;//���荞�݋֎~

	int error;//�G���[����p�ϐ�

			  //���蓖�ăs����ݒ�
	P1.DDR = 0b11111111;//�|�[�g1���g�p
	P1.DR.BIT.B0 = 1;//�s��0��SCL
	P1.DR.BIT.B1 = 1;//�s��1��SDA
	i2cinterval();
	P1.DR.BIT.B2 = 1;//�s��2��d���ɂ���,������EEPROM�I��
	i2cwait();

	//�R���g���[���f�[�^���M�X�^�[�g
	P1.DR.BIT.B1 = 0;//startbit
	i2cinterval();
	P1.DR.BIT.B0 = 0;//clockstart
	i2cdelay();

	//�R���g���[���o�C�g(����)
	sendbyte(0b10100000);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//��ʃA�h���X�o�C�g��0x00
	sendbyte(0x00);
	error = confirmACK();
	if (error)return 1;

	//���ʃA�h���X�o�C�g��0x00
	sendbyte(64 * page);
	error = confirmACK();
	if (error)return 1;

	//startbit����
	P1.DDR = 0b11111111;
	P1.DR.BIT.B0 = 1;
	P1.DR.BIT.B1 = 1;
	i2cinterval();
	P1.DR.BIT.B1 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	i2cdelay();

	//�R���g���[���o�C�g(�ǂ�)
	sendbyte(0b10100001);
	error = confirmACK();
	if (error) return 1;
	i2cinterval();

	//1�o�C�g���ǂ݂���,Datas[]�Ɋi�[
	int i;
	for (i = 0; i < 63; i++)//64��ڂ͓���Ȃ̂ŏ��O
	{
		Datas[i] = readbyte();
		sendACK(0);
	}
	Datas[63] = readbyte();
	sendACK(1);
	P1.DDR = 0b11111111;
	i2cdelay();

	//�ǂݍ��ݏI��
	stopbit();
	i2cwait();

	//EEPROM�̓d��0ff
	P1.DR.BIT.B2 = 0;
	i2cinterval();
	P1.DR.BIT.B0 = 0;
	P1.DR.BIT.B1 = 0;
	i2cdelay();

	return 0;
}

//�p���x���O���p�֐����`,�Ԃ�l�͐���0,���s1
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

//���[�^�[���O���p�֐����`,�Ԃ�l�͐���0,���s1
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

//���ˑ��u���O���p�֐����`,�Ԃ�l�͐���0,���s1
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

//�c�蔭�ː��\�����O���p�֐����`,�Ԃ�l�͐���0,���s1
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