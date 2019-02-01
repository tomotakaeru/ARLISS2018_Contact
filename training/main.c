//真のmainファイル

#include <3052.h>
#include "stepping_motor.h"
#include "seg.h"
#include "I2CEEPROM.h"
#include "I2CCommon.h"
#include "EEPROMinstructions.h"
#include "xpunch.h"

int omega_f, omega1, omega2;
int EEPROMAddress = 0;//とりあえず書き込みは最大256*6バイト
int Datas[64];//dataを読み出すときの入れ物

/*omega_datas[2]
1つ目は構造体の回転数
2つ目は構造体の回転方向*/
unsigned int omega_datas[2];

/*motordata[3]:モーターと発射回数のログをとるための入れ物
1つ目はステッピングモーターの回転方向、時計回りが1、反時計回りが0
2つ目はステッピングモーターのステップ数、前回の位置に対してどれだけ動かしたか
3つ目は発射回数、動作開始からの累計発射数*/
int motordata[3] = { 0,0,0 };

/*ここら辺にexternのやつあったら書いといてほしい*/
//extern
extern unsigned int cnt_ch2;

void int_irq1(void);    // 本当のメイン

//割り込みIRQ1
void int_irq1(void) {
	INTC.IER.BIT.IRQ0E = 0;//interrupt disable(IRQ0)
	INTC.IER.BIT.IRQ1E = 0;//interrupt disable(IRQ1)

	wait_ms(4, 500);

	//開始ログ
	SCI1_PRINTF("start\n");
	eepromwrite8byte(1, 1, 1, 1, 1, 1, 1, 1);//start!
	CalculateAngularVelocityInit();

	//まずは連射する。この関数の中でステッピングモーターを回す必要あり。
	SCI1_PRINTF("first attack\n");
	FirstAttack();

	//次に、回転数が500以下になるまで連射。
	while (omega_datas[0] > 500) {
		SCI1_PRINTF("next attack\n");
		NextAttack();
	}

	//回転数が500以下になったら、止めるまでにあと何発必要か予測して表示。
	PredictBulletsToFire();

	//最後に、検知できなくなるまで発射
	int end = 0;
	while (end == 0) {
		end = LastAttack();
	}
	//終了ログ
	SCI1_PRINTF("end\n");
	eepromwrite8byte(255, 255, 255, 255, 255, 255, 255, 255);//end!
	seg(777);
}

void main(void) {
	SYSCR.BIT.UE = 1;
	INTC.ISCR.BIT.IRQ0SC = 1;//入力立下りで割り込み
	INTC.IPRA.BIT._IRQ0 = 1;//priority level 1
	INTC.ISR.BIT.IRQ0F = 0;//割り込みステータスクリア
	INTC.IER.BIT.IRQ0E = 1;//interrupt enable(IRQ0)
	INTC.ISCR.BIT.IRQ1SC = 1;//入力立下りで割り込み
	INTC.IPRA.BIT._IRQ1 = 1;//priority level 1
	INTC.ISR.BIT.IRQ1F = 0;//割り込みステータスクリア
	INTC.IER.BIT.IRQ1E = 1;//interrupt enable(IRQ0)
	EI;

	while (1);//ピンの割り込みがあるまでは何もしない
}