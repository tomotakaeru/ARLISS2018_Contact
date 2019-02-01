//�^��main�t�@�C��

#include <3052.h>
#include "stepping_motor.h"
#include "seg.h"
#include "I2CEEPROM.h"
#include "I2CCommon.h"
#include "EEPROMinstructions.h"
#include "xpunch.h"

int omega_f, omega1, omega2;
int EEPROMAddress = 0;//�Ƃ肠�����������݂͍ő�256*6�o�C�g
int Datas[64];//data��ǂݏo���Ƃ��̓��ꕨ

/*omega_datas[2]
1�ڂ͍\���̂̉�]��
2�ڂ͍\���̂̉�]����*/
unsigned int omega_datas[2];

/*motordata[3]:���[�^�[�Ɣ��ˉ񐔂̃��O���Ƃ邽�߂̓��ꕨ
1�ڂ̓X�e�b�s���O���[�^�[�̉�]�����A���v��肪1�A�����v��肪0
2�ڂ̓X�e�b�s���O���[�^�[�̃X�e�b�v���A�O��̈ʒu�ɑ΂��Ăǂꂾ������������
3�ڂ͔��ˉ񐔁A����J�n����̗݌v���ː�*/
int motordata[3] = { 0,0,0 };

/*������ӂ�extern�̂�������珑���Ƃ��Ăق���*/
//extern
extern unsigned int cnt_ch2;

void int_irq1(void);    // �{���̃��C��

//���荞��IRQ1
void int_irq1(void) {
	INTC.IER.BIT.IRQ0E = 0;//interrupt disable(IRQ0)
	INTC.IER.BIT.IRQ1E = 0;//interrupt disable(IRQ1)

	wait_ms(4, 500);

	//�J�n���O
	SCI1_PRINTF("start\n");
	eepromwrite8byte(1, 1, 1, 1, 1, 1, 1, 1);//start!
	CalculateAngularVelocityInit();

	//�܂��͘A�˂���B���̊֐��̒��ŃX�e�b�s���O���[�^�[���񂷕K�v����B
	SCI1_PRINTF("first attack\n");
	FirstAttack();

	//���ɁA��]����500�ȉ��ɂȂ�܂ŘA�ˁB
	while (omega_datas[0] > 500) {
		SCI1_PRINTF("next attack\n");
		NextAttack();
	}

	//��]����500�ȉ��ɂȂ�����A�~�߂�܂łɂ��Ɖ����K�v���\�����ĕ\���B
	PredictBulletsToFire();

	//�Ō�ɁA���m�ł��Ȃ��Ȃ�܂Ŕ���
	int end = 0;
	while (end == 0) {
		end = LastAttack();
	}
	//�I�����O
	SCI1_PRINTF("end\n");
	eepromwrite8byte(255, 255, 255, 255, 255, 255, 255, 255);//end!
	seg(777);
}

void main(void) {
	SYSCR.BIT.UE = 1;
	INTC.ISCR.BIT.IRQ0SC = 1;//���͗�����Ŋ��荞��
	INTC.IPRA.BIT._IRQ0 = 1;//priority level 1
	INTC.ISR.BIT.IRQ0F = 0;//���荞�݃X�e�[�^�X�N���A
	INTC.IER.BIT.IRQ0E = 1;//interrupt enable(IRQ0)
	INTC.ISCR.BIT.IRQ1SC = 1;//���͗�����Ŋ��荞��
	INTC.IPRA.BIT._IRQ1 = 1;//priority level 1
	INTC.ISR.BIT.IRQ1F = 0;//���荞�݃X�e�[�^�X�N���A
	INTC.IER.BIT.IRQ1E = 1;//interrupt enable(IRQ0)
	EI;

	while (1);//�s���̊��荞�݂�����܂ł͉������Ȃ�
}