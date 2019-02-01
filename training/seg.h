#ifndef _SEG_H_
#define _SEG_H_


void seg_sub(int channel, int num);
void seg(int number);

void seg(int number) {
	PB.DDR = 0xff;              // 出力設定
	P1.DDR = 0xff;              // 出力設定

	if (number > 1000) {
		seg_sub(0, 0);
		seg_sub(1, 10);
		seg_sub(2, 10);
		return;
	}
	else if (number == 1000) {
		seg_sub(0, 10);
		seg_sub(1, 0);
		seg_sub(2, 0);
		return;
	}
	else {
		int a, b, c;              // a:100,b:10,c:1
		a = number / 100;
		b = (number - a * 100) / 10;
		c = (number - a * 100 - b * 10);    // 引数を位別の数字に分ける

		if (a == 0 && b == 0) {
			seg_sub(0, a);
			seg_sub(1, 10);
			seg_sub(2, c);
		}
		else {
			seg_sub(0, a);
			seg_sub(1, b);
			seg_sub(2, c);
		}
		return;
	}
}


void seg_sub(int channel, int num) {
	switch (channel) {
	case 0:
		switch (num) {
		case 0:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 1;   //B
			PB.DR.BIT.B2 = 1;   //C
			PB.DR.BIT.B3 = 1;   //D
			break;
		case 1:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 2:
			PB.DR.BIT.B0 = 0;   //A
			PB.DR.BIT.B1 = 1;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 3:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 1;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 4:
			PB.DR.BIT.B0 = 0;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 1;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 5:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 1;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 6:
			PB.DR.BIT.B0 = 0;   //A
			PB.DR.BIT.B1 = 1;   //B
			PB.DR.BIT.B2 = 1;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 7:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 1;   //B
			PB.DR.BIT.B2 = 1;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		case 8:
			PB.DR.BIT.B0 = 0;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 1;   //D
			break;
		case 9:
			PB.DR.BIT.B0 = 1;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 1;   //D
			break;
		case 10:
			PB.DR.BIT.B0 = 0;   //A
			PB.DR.BIT.B1 = 0;   //B
			PB.DR.BIT.B2 = 0;   //C
			PB.DR.BIT.B3 = 0;   //D
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (num) {
		case 0:
			PB.DR.BIT.B4 = 0;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 1:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 2:
			PB.DR.BIT.B4 = 0;   //A
			PB.DR.BIT.B5 = 1;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 3:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 1;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 4:
			PB.DR.BIT.B4 = 0;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 1;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 5:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 1;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 6:
			PB.DR.BIT.B4 = 0;   //A
			PB.DR.BIT.B5 = 1;   //B
			PB.DR.BIT.B6 = 1;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 7:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 1;   //B
			PB.DR.BIT.B6 = 1;   //C
			PB.DR.BIT.B7 = 0;   //D
			break;
		case 8:
			PB.DR.BIT.B4 = 0;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 1;   //D
			break;
		case 9:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 0;   //B
			PB.DR.BIT.B6 = 0;   //C
			PB.DR.BIT.B7 = 1;   //D
			break;
		case 10:
			PB.DR.BIT.B4 = 1;   //A
			PB.DR.BIT.B5 = 1;   //B
			PB.DR.BIT.B6 = 1;   //C
			PB.DR.BIT.B7 = 1;   //D
		default:
			break;
		}
		break;
	case 2:
		switch (num) {
		case 0:
			P1.DR.BIT.B4 = 0;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 1:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 2:
			P1.DR.BIT.B4 = 0;   //A
			P1.DR.BIT.B5 = 1;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 3:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 1;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 4:
			P1.DR.BIT.B4 = 0;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 1;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 5:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 1;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 6:
			P1.DR.BIT.B4 = 0;   //A
			P1.DR.BIT.B5 = 1;   //B
			P1.DR.BIT.B6 = 1;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 7:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 1;   //B
			P1.DR.BIT.B6 = 1;   //C
			P1.DR.BIT.B7 = 0;   //D
			break;
		case 8:
			P1.DR.BIT.B4 = 0;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 1;   //D
			break;
		case 9:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 0;   //B
			P1.DR.BIT.B6 = 0;   //C
			P1.DR.BIT.B7 = 1;   //D
			break;
		case 10:
			P1.DR.BIT.B4 = 1;   //A
			P1.DR.BIT.B5 = 1;   //B
			P1.DR.BIT.B6 = 1;   //C
			P1.DR.BIT.B7 = 1;   //D
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

#endif