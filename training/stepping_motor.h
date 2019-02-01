#ifndef _STEPPING_MOTOR_H_
#define _STEPPING_MOTOR_H_

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

static char rxb[RX_BUFFER_SIZE];  // 受信用バッファを確保
static char txb[TX_BUFFER_SIZE];  // 送信用バッファを確保

void initITU(void);
void wait_ms(int channel,float ms);
void rot(int steps,int cw_ccw,float Hz);
void sm_state(int state);
void dcmotor_switch(int on_off);
void fire(float fire_time);
int state = 1;
void tuna_set(void);

extern int motordata[];

// tuna setting
void tuna_set(void){
    SCI1_INIT(br19200, txb, TX_BUFFER_SIZE, rxb, RX_BUFFER_SIZE); // SCI1機能を初期化
    EI; // 割り込み許可
}

//  DCモーターON/OFF
void dcmotor_switch(int on_off){
	P2.DDR = 0xff;          // 出力設定
	P2.DR.BIT.B0 = on_off;	// 1でモーターON,０でOFF
}

// 射出
void fire(float fire_time){
	P2.DDR = 0xff;          // 出力設定
	P2.DR.BIT.B1 = 1;		// ON
	wait_ms(4,fire_time);	// 少し待つ
	P2.DR.BIT.B1 = 0;		// OFF
}

//ステッピングモーターの状態セット
void sm_state(int sm_state){
	switch(sm_state){
        case 0:
        state = 4;
        P5.DR.BIT.B0 = 0;   //IN-A
        P5.DR.BIT.B1 = 1;   //IN-B
        break;
        case 1:
        P5.DR.BIT.B0 = 0;   //IN-A
        P5.DR.BIT.B1 = 0;   //IN-B
        break;
        case 2:
        P5.DR.BIT.B0 = 1;   //IN-A
        P5.DR.BIT.B1 = 0;   //IN-B
        break;
        case 3:
        P5.DR.BIT.B0 = 1;   //IN-A
        P5.DR.BIT.B1 = 1;   //IN-B
        break;
        case 4:
        P5.DR.BIT.B0 = 0;   //IN-A
        P5.DR.BIT.B1 = 1;   //IN-B
        break;
        case 5:
        state = 1;
        P5.DR.BIT.B0 = 0;   //IN-A
        P5.DR.BIT.B1 = 0;   //IN-B
        break;
        default:
        break;       
    }
}

//指定ステップ回転
void rot(int steps,int cw_ccw,float Hz)	{
//steps:回したいステップ数、cw_ccw:時計なら1、逆時計なら0 Hz:パルス周波数
//1step 1.25deg	
	motordata[0] = cw_ccw;
	motordata[1] = steps;
	P5.DDR = 0xff;    	// 出力設定
	int i;
    float T = 1000.0/Hz;
	P5.DR.BIT.B2 = 1;	//POWER ON
    if (cw_ccw==1){
    	for (i=0;i<steps;i++){
    		wait_ms(4,T);
    		state += 1;
    		sm_state(state);
    	}
    }
    else if (cw_ccw==0){
    	for (i=0;i<steps;i++){
    		wait_ms(4,T);
    		state -= 1;
    		sm_state(state);
    	}
    }
    else
    return;   
}

// ITU settings
void initITU(void)
{
    ITU.TSNC.BIT.SYNC0 = 0;         // 他チャンネルとの同期無し（独立動作）
    ITU.TSNC.BIT.SYNC1 = 0;         // 他チャンネルとの同期無し（独立動作）
    ITU.TSNC.BIT.SYNC2 = 0;         // 他チャンネルとの同期無し（独立動作）
    ITU.TSNC.BIT.SYNC3 = 0;         // 他チャンネルとの同期無し（独立動作）
    ITU.TSNC.BIT.SYNC4 = 0;         // 他チャンネルとの同期無し（独立動作）

    ITU0.TCR.BYTE = 0x40;           // Φでカウント,GRB一致でリセット
    ITU1.TCR.BYTE = 0x40;           // Φでカウント,GRB一致でリセット
    ITU2.TCR.BYTE = 0x40;           // Φでカウント,GRB一致でリセット
    ITU3.TCR.BYTE = 0x40;           // Φでカウント,GRB一致でリセット
    ITU4.TCR.BYTE = 0x40;           // Φでカウント,GRB一致でリセット

    ITU0.TIOR.BYTE = 0;             // GRの制御
    ITU1.TIOR.BYTE = 0;             // GRの制御
    ITU2.TIOR.BYTE = 0;             // GRの制御
    ITU3.TIOR.BYTE = 0;             // GRの制御
    ITU4.TIOR.BYTE = 0;             // GRの制御
     
    ITU0.TIER.BIT.IMIEA = 0;        // 割り込み禁止(ｵｰﾊﾞｰﾌﾛｰ/IMFA/IMFB)
    ITU1.TIER.BIT.IMIEA = 0;        // 割り込み禁止(ｵｰﾊﾞｰﾌﾛｰ/IMFA/IMFB)
    ITU2.TIER.BIT.IMIEA = 0;        // 割り込み禁止(ｵｰﾊﾞｰﾌﾛｰ/IMFA/IMFB)
    ITU3.TIER.BIT.IMIEA = 0;        // 割り込み禁止(ｵｰﾊﾞｰﾌﾛｰ/IMFA/IMFB)
    ITU4.TIER.BIT.IMIEA = 0;        // 割り込み禁止(ｵｰﾊﾞｰﾌﾛｰ/IMFA/IMFB)
}

// timer
void wait_ms(int channel,float ms)
{
    int i,int_ms,new_ms;
    float deci_ms;
    int_ms = ms;
    deci_ms = ms - int_ms;
    new_ms = 25000*deci_ms;
    switch(channel){
        case 0:
        ITU.TMDR.BIT.PWM0 = 0;          //PWMモード解除
        ITU0.GRB = 25000;               //1msに設定
        for (i=0;i<int_ms;i++){
            ITU0.TCNT = 0;              //カウントリセット
            ITU.TSTR.BIT.STR0 = 1;      //スタート
            while (!ITU0.TSR.BIT.IMFB); //時間待ち
            ITU0.TSR.BIT.IMFB = 0;      //IMFBクリア
            ITU.TSTR.BIT.STR0 = 0;      //タイマを停止
        }
        ITU0.GRB = new_ms;              //対応するmsに設定
        ITU0.TCNT = 0;                  //カウントリセット
        ITU.TSTR.BIT.STR0 = 1;          //スタート
        while (!ITU0.TSR.BIT.IMFB);     //時間待ち
        ITU0.TSR.BIT.IMFB = 0;          //IMFBクリア
        ITU.TSTR.BIT.STR0 = 0;          //タイマを停止
        break;
        case 1:
        ITU.TMDR.BIT.PWM1 = 0;          //PWMモード解除
        ITU1.GRB = 25000;               //1msに設定
        for (i=0;i<int_ms;i++){
            ITU1.TCNT = 0;              //カウントリセット
            ITU.TSTR.BIT.STR1 = 1;      //スタート
            while (!ITU1.TSR.BIT.IMFB); //時間待ち
            ITU1.TSR.BIT.IMFB = 0;      //IMFBクリア
            ITU.TSTR.BIT.STR1 = 0;      //タイマを停止
        }
        ITU1.GRB = new_ms;              //対応するmsに設定
        ITU1.TCNT = 0;                  //カウントリセット
        ITU.TSTR.BIT.STR1 = 1;          //スタート
        while (!ITU1.TSR.BIT.IMFB);     //時間待ち
        ITU1.TSR.BIT.IMFB = 0;          //IMFBクリア
        ITU.TSTR.BIT.STR1 = 0;          //タイマを停止
        break;  
        case 2:
        ITU.TMDR.BIT.PWM2 = 0;          //PWMモード解除
        ITU2.GRB = 25000;               //1msに設定
        for (i=0;i<int_ms;i++){
            ITU2.TCNT = 0;              //カウントリセット
            ITU.TSTR.BIT.STR2 = 1;      //スタート
            while (!ITU2.TSR.BIT.IMFB); //時間待ち
            ITU2.TSR.BIT.IMFB = 0;      //IMFBクリア
            ITU.TSTR.BIT.STR2 = 0;      //タイマを停止
        }
        ITU2.GRB = new_ms;              //対応するmsに設定
        ITU2.TCNT = 0;                  //カウントリセット
        ITU.TSTR.BIT.STR2 = 1;          //スタート
        while (!ITU2.TSR.BIT.IMFB);     //時間待ち
        ITU2.TSR.BIT.IMFB = 0;          //IMFBクリア
        ITU.TSTR.BIT.STR2 = 0;          //タイマを停止
        break;  
        case 3:
        ITU.TMDR.BIT.PWM3 = 0;          //PWMモード解除
        ITU3.GRB = 25000;               //1msに設定
        for (i=0;i<int_ms;i++){
            ITU3.TCNT = 0;              //カウントリセット
            ITU.TSTR.BIT.STR3 = 1;      //スタート
            while (!ITU3.TSR.BIT.IMFB); //時間待ち
            ITU3.TSR.BIT.IMFB = 0;      //IMFBクリア
            ITU.TSTR.BIT.STR3 = 0;      //タイマを停止
        }
        ITU3.GRB = new_ms;              //対応するmsに設定
        ITU3.TCNT = 0;                  //カウントリセット
        ITU.TSTR.BIT.STR3 = 1;          //スタート
        while (!ITU3.TSR.BIT.IMFB);     //時間待ち
        ITU3.TSR.BIT.IMFB = 0;          //IMFBクリア
        ITU.TSTR.BIT.STR3 = 0;          //タイマを停止
        break;    
        case 4:
        ITU.TMDR.BIT.PWM4 = 0;          //PWMモード解除
        ITU4.GRB = 25000;               //1msに設定
        for (i=0;i<int_ms;i++){
            ITU4.TCNT = 0;              //カウントリセット
            ITU.TSTR.BIT.STR4 = 1;      //スタート
            while (!ITU4.TSR.BIT.IMFB); //時間待ち
            ITU4.TSR.BIT.IMFB = 0;      //IMFBクリア
            ITU.TSTR.BIT.STR4 = 0;      //タイマを停止
        }
        ITU4.GRB = new_ms;              //対応するmsに設定
        ITU4.TCNT = 0;                  //カウントリセット
        ITU.TSTR.BIT.STR4 = 1;          //スタート
        while (!ITU4.TSR.BIT.IMFB);     //時間待ち
        ITU4.TSR.BIT.IMFB = 0;          //IMFBクリア
        ITU.TSTR.BIT.STR4 = 0;          //タイマを停止
        break;  
        default:
        break; 
    }
}   

#endif