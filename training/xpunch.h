#ifndef _XPUNCH_H_
#define _XPUNCH_H_

#include <3052.h>
#include "stepping_motor.h"
#include "seg.h"
#include "I2CEEPROM.h"

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

static char rxb[RX_BUFFER_SIZE];            //受信用バッファを確保
static char txb[TX_BUFFER_SIZE];            //送信用バッファを確保

void FirstAttack();
void NextAttack();
int LastAttack();
int CalculateAngularVelocity(void);
void CalculateAngularVelocityInit();
void itu2int(void);
void int_imia2(void);
void omega_search(void);
void adint(void);
void ituint(void);
void dc_ctr_multi(int punches);
void dc_ctr_last(int punches);
void SuccessiveFire();

int v0, v1, v0_0, v1_0;
int i0, i1;
int check_v0, check_v1;
int cnt = 0;
unsigned int cnt_ch2_0 = 0, cnt_ch2 = 0;
unsigned long rot_speed;    //1000倍の値入れたやつ
int v0_max_1 = 0, v1_max_1 = 0;
int v_sub, v0_max_0, v1_max_0;
int cnt1 = 0, cnt2 = 0, clock_dir, itu_str = 0, restr = 0;
int i;
int bulletsToFire = 0;
int currentStep = 0;
unsigned int initialRotationDirection;

extern unsigned int omega_datas[];//戻り値二つは出せないから、データ箱を用意
extern int motordata[];

void FirstAttack() {
	//回転の観測
	wait_ms(4, 500);
	int error = 1;
	while (error) {
		error = CalculateAngularVelocity();
	}
	SCI1_PRINTF("%d\t%d\t%d\n", cnt_ch2, omega_datas[0], omega_datas[1]);

	//ここでステッピングモーターを回す
	rot(3, omega_datas[1], 5);
	currentStep = 3;
	initialRotationDirection = omega_datas[1];

	//回転のログ取り
	eepromwriteomegaandtime();

	//7seg表示
	seg(omega_datas[1]);
	wait_ms(4, 1000);
	seg(omega_datas[0] / 10);
	wait_ms(4, 2000);

	//発射
	SuccessiveFire();

	//発射のログ取り
	eepromwritefirearm();

	//再び回転を観測
	wait_ms(4, 500);
	CalculateAngularVelocity();
	SCI1_PRINTF("%d\t%d\t%d\n", cnt_ch2, omega_datas[0], omega_datas[1]);

	//再び回転のログ取り
	eepromwriteomegaandtime();

	//7seg表示
	seg(omega_datas[1]);
	wait_ms(4, 1000);
	seg(omega_datas[0] / 10);
	wait_ms(4, 2000);
}

void NextAttack() {
	//FirstAttackの最後のデータをもとに発射
	SuccessiveFire();

	//発射のログ取り(ステッピングモーターを動かさないほう)
	eepromwritesuccessive();

	//回転を観測
	wait_ms(4, 500);
	CalculateAngularVelocity();
	SCI1_PRINTF("%d\t%d\t%d\n", cnt_ch2, omega_datas[0], omega_datas[1]);

	//回転のログ取り
	eepromwriteomegaandtime();

	//7seg表示
	seg(omega_datas[1]);
	wait_ms(4, 1000);
	seg(omega_datas[0] / 10);
	wait_ms(4, 2000);
}

void PredictBulletsToFire() {
	//7セグ表示
	seg(1000);
	wait_ms(4, 2000);

	//取得したデータをもとに、残り発射数を予測
	//残り発射数
	bulletsToFire = (omega_datas[0] + 30) / 100;
	if (bulletsToFire > 4) {
		bulletsToFire = 4;
	}

	//7seg表示
	seg(bulletsToFire + 10);

	//予測データのログ取り
	eepromWriteBulletsToFire(bulletsToFire);
}

int LastAttack() {
	if (omega_datas[1] != initialRotationDirection) {
		rot(currentStep * 2, omega_datas[1], 5);
		initialRotationDirection = omega_datas[1];

		//発射のログ取り(ステッピングモーターを動かすほう)
		eepromwritefirearm();
	}

	if (omega_datas[0] > 220) {
		dc_ctr_last(1);

		//発射のログ取り(ステッピングモーターを動かさないほう)
		eepromwritesuccessive();
	}
	else if (omega_datas[0] > 170) {
		if (currentStep == 3) {
			rot(1, !omega_datas[1], 5);
			dc_ctr_last(2);
			currentStep = 2;

			//発射のログ取り(ステッピングモーターを動かすほう)
			eepromwritefirearm();
		}
		else{
			dc_ctr_last(2);

			//発射のログ取り(ステッピングモーターを動かさないほう)
			eepromwritesuccessive();
		}
	}
	else if (omega_datas[0] > 130) {
		if (currentStep == 2) {
			rot(1, omega_datas[1], 5);
			dc_ctr_last(1);
			currentStep = 3;

			//発射のログ取り(ステッピングモーターを動かすほう)
			eepromwritefirearm();
		}
		else {
			dc_ctr_last(1);

			//発射のログ取り(ステッピングモーターを動かさないほう)
			eepromwritesuccessive();
		}
	}
	else {
		if (currentStep == 3) {
			rot(1, !omega_datas[1], 5);
			dc_ctr_last(1);
			currentStep = 2;

			//発射のログ取り(ステッピングモーターを動かすほう)
			eepromwritefirearm();
		}
		else {
			dc_ctr_last(1);

			//発射のログ取り(ステッピングモーターを動かさないほう)
			eepromwritesuccessive();
		}
	}

	//回転を観測
	wait_ms(4, 500);
	int end = CalculateAngularVelocity();
	if (end){
		omega_datas[0] = 0;
		SCI1_PRINTF("%d\t%d\n", cnt_ch2, omega_datas[0]);
		eepromwriteomegaandtime();
		return end;
	}
	SCI1_PRINTF("%d\t%d\t%d\n", cnt_ch2, omega_datas[0], omega_datas[1]);

	//回転のログ取り
	eepromwriteomegaandtime();

	//7seg表示
	seg(omega_datas[1]);
	wait_ms(4, 1000);
	seg(omega_datas[0] / 10);
	wait_ms(4, 2000);

	return end;
}

int CalculateAngularVelocity(void) {
	int j = 0;
	if (j < 2) {
		adint();
		ituint();
		while (cnt2 == 0) {
			AD.ADCSR.BIT.ADST = 1;              //A/D変換開始
			while (AD.ADCSR.BIT.ADF == 0);       //A/D変換中待機
			i0 = AD.ADDRA >> 6;                 //AN4からの入力値
			i1 = AD.ADDRB >> 6;                 //AN5からの入力値
			v0_0 = (i0 + 1)*(5.0 / 1024) * 1000;               //AN4からの入力電圧,intでないと送れないのでmV単位
			v1_0 = (i1 + 1)*(5.0 / 1024) * 1000;               //AN5からの入力電圧
			v_sub = v0_0 - v1_0;                      //差をとる
			if (v_sub >= 0) {
				v0 = v_sub;
				v1 = 0;
			}
			else {
				v0 = 0;
				v1 = -v_sub;
			}
			if (v0 > v0_max_1) {
				v0_max_1 = v0;
			}
			if (v1 > v1_max_1) {
				v1_max_1 = v1;
			}
			if (v0 > check_v0 && cnt1 != 0) {
				cnt2 = cnt;
				ITU.TSTR.BIT.STR0 = 0;          //タイマストップitu_str = 0;
			}
			if (v1 > check_v1 && cnt1 == 0 && itu_str == 1) {
				cnt1 = cnt;
				cnt = 0;
				//wait_ms(4,3000);                //次のifをすぐ作動させないため(本番はv1も取るから問題なし)
			}
			if (v0 > check_v0 && cnt == 0 && itu_str == 0) {
				ITU.TSTR.BIT.STR0 = 1;          //タイマスタート
				itu_str = 1;
				//wait_ms(4,3000);                //次のifをすぐ作動させないため(本番はv1も取るから問題なし)
			}

			AD.ADCSR.BIT.ADF = 0;               //A/D変換終了フラグをクリア
			wait_ms(4, 5);                     //パルスが通りすぎる間0.005s待つ(本番はv1も取るから問題なし)

			if (cnt > 10000) {//timerstartのち検知されずに10s経ったら0を返す
				ITU.TSTR.BIT.STR0 = 0;          //タイマストップitu_str = 0;
				ITU0.TCNT = 0; //カウントリセット
				seg(888);
				//SCI1_PRINTF("restart %d\n", restr);
				//omega_search();
				v0_max_1 = 0;
				v1_max_1 = 0;
				cnt1 = 0;
				cnt2 = 0;
				itu_str = 0;
				cnt = 0;
				return 1;
			}				
		}

		rot_speed = 1000 / (0.001*(cnt1 + cnt2) * 3);               //intでないと送れないので1000倍している

		if (cnt1 < cnt2) {
			clock_dir = 1;          //時計回り
		}
		else {
			clock_dir = 0;          //反時計回り
		}

		omega_datas[0] = rot_speed;
		omega_datas[1] = !clock_dir;
		check_v0 = v0_max_1 * 0.5;
		check_v1 = v1_max_1 * 0.5;
		v0_max_1 = 0;
		v1_max_1 = 0;
		cnt1 = 0;
		cnt2 = 0;
		itu_str = 0;
		cnt = 0;
		j++;
	}
	return 0;
}
void SuccessiveFire(){
	//角速度に応じて発射数を決める
	if (omega_datas[0] > 3000) {
		dc_ctr_multi(20);
	}
	else if (omega_datas[0] > 1000) {
		dc_ctr_multi((omega_datas[0] - 1000) / 130 + 5);
	}
	else if (omega_datas[0] > 500) {
		dc_ctr_multi((omega_datas[0] - 500) / 100);
	}
}

void dc_ctr_multi(int punches) {
	int i;
	dcmotor_switch(1);
	for (i = 0; i < punches; i++) {
		seg(punches - i);
		fire(100);
		wait_ms(4, 650);
	}
	dcmotor_switch(0);
	seg(0);
	wait_ms(4, 100);
	motordata[2] += punches;
}

void dc_ctr_last(int punches) {
	int i;
	dcmotor_switch(1);
	for (i = 0; i < punches; i++) {
		fire(100);
		wait_ms(4, 650);
	}
	dcmotor_switch(0);
	wait_ms(4, 100);
	motordata[2] += punches;
	bulletsToFire -= punches;
	seg(bulletsToFire + 10);
}

void CalculateAngularVelocityInit() {
	tuna_set();
	adint();
	ituint();
	itu2int();

	ITU2.TCNT = 0;                  //カウントリセット
	ITU.TSTR.BIT.STR2 = 1;          //タイマスタート
	wait_ms(4, 100);
	omega_search();

	SCI1_PRINTF("total_10msecs\trot_speed\n");
}

void omega_search(void) {
	adint();
	ituint();
	EI;                                   //ITUの割り込みがあるから常に許可？
	wait_ms(4, 100);          //reset押したときのピークの影響を排除するため
// 最大値をとる
	v0_max_0 = 0;
	v1_max_0 = 0;
	ITU.TSTR.BIT.STR0 = 1;          //タイマスタート
	while (cnt < 1000) {
		AD.ADCSR.BIT.ADST = 1;              //A/D変換開始
		while (AD.ADCSR.BIT.ADF == 0);       //A/D変換中待機
		i0 = AD.ADDRA >> 6;                 //AN4からの入力値
		i1 = AD.ADDRB >> 6;                 //AN5からの入力値
		v0_0 = (i0 + 1)*(5.0 / 1024) * 1000;      //AN4からの入力電圧,intでないと送れないのでmV単位
		v1_0 = (i1 + 1)*(5.0 / 1024) * 1000;      //AN5からの入力電圧
		v_sub = v0_0 - v1_0;                      //差をとる
		if (v_sub >= 0) {
			v0 = v_sub;
			v1 = 0;
		}
		else {
			v0 = 0;
			v1 = -v_sub;
		}
		if (v0 > v0_max_0) {
			v0_max_0 = v0;
		}
		if (v1 > v1_max_0) {
			v1_max_0 = v1;
		}
		AD.ADCSR.BIT.ADF = 0;               //A/D変換終了フラグをクリア
		wait_ms(4, 5);                     //パルスが通りすぎる間0.005s待つ(本番はv1も取るから問題なし)
	}
	ITU.TSTR.BIT.STR0 = 0;          //タイマストップ
	ITU0.TCNT = 0;                  //カウントリセット
	cnt = 0;                        //リセット
	check_v0 = v0_max_0 * 0.5;
	check_v1 = v1_max_0 * 0.5;
}

void int_imia0(void)                        //タイマ割り込み
{
	ITU0.TSR.BIT.IMFA = 0;                //IMFAフラグを折る
	cnt++;
}

void adint(void)                         //A/Dコンバータの設定
{
	AD.ADCSR.BIT.ADF = 0;                //A/D変換終了フラグをクリア
	AD.ADCSR.BIT.SCAN = 1;               //スキャンモードに設定
	AD.ADCSR.BIT.CKS = 1;                //変換時間を128ステートに設定
	AD.ADCSR.BIT.CH = 5;                 //チャネルをAN4,5に設定
	AD.ADCSR.BIT.ADIE = 0;               //変換後の割り込み要求を許可←1だとテストで動作せず不明
}

void ituint(void)                        //ITUの設定
{
	ITU0.TCR.BIT.CCLR = 1;               //カウンタクリア要因をGRAのコンペアマッチに設定
	ITU0.TCR.BIT.TPSC = 3;               //内部クロックの1/8でカウント 25MHz/8 = 3.125MHz 0.32μs
	ITU0.GRA = 3124;                     //3125*0.32μs = 1ms
	ITU0.TIER.BIT.IMIEA = 1;             //IMFAフラグによる割り込み許可
}

void int_imia2(void)                        //タイマ割り込み
{
	ITU2.TSR.BIT.IMFA = 0;                //IMFAフラグを折る
	cnt_ch2_0++;
	if (cnt_ch2_0 == 10) {
		cnt_ch2++;
		cnt_ch2_0 = 0;
	}
}

void itu2int(void) {
	ITU2.TCR.BIT.CCLR = 1;               //カウンタクリア要因をGRAのコンペアマッチに設定
	ITU2.TCR.BIT.TPSC = 3;               //内部クロックの1/8でカウント 25MHz/8 = 3.125MHz 0.32μs
	ITU2.GRA = 3124;                     //3125*0.32μs = 1ms
	ITU2.TIER.BIT.IMIEA = 1;             //IMFAフラグによる割り込み許可
	ITU2.TCNT = 0;                      //カウントリセット
}

#endif 