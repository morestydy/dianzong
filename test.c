#include<STC12C5A60S2.h>

#define uint unsigned int
#define uchar unsigned char
	
uchar code wx[4] = {0x7f,0xbf,0xdf,0xef};
uchar code smg[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar point;
uchar bit_num[4];
uint nSpeed = 0,cSetSpeed = 10;
uint nCount = 0;
uchar cDuty = 10; //10%占空比
uint nDelayKey;
uchar cKey;
uchar cKeyCode;
bit bStill;
sbit PWM1=P1^0;	
sbit PWM2=P1^1;
uchar cCount;

/******************** 按键处理程序 ******************/
void DisposeKEY()
{
	switch(cKeyCode)
	{
		case 0x0c:
		if(cSetSpeed < 99)cSetSpeed++;
		nDelayKey = 2500; 	
		break;
	case 0x14:
		if(cSetSpeed >10)cSetSpeed--;
		nDelayKey = 2500;
		break;
	}
	bit_num[1] = smg[cSetSpeed / 10];
	bit_num[0] = smg[cSetSpeed % 10];
	cKeyCode = 0;
}
void main(void)
{
	P0M0 = 0xff;
	TMOD = 0x51;
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值 100us
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	TR1 = 1;
	PWM1 = 0;

	bit_num[3] = smg[nSpeed / 10];
	bit_num[2] = smg[nSpeed % 10];
	bit_num[1] = smg[cSetSpeed / 10];
	bit_num[0] = smg[cSetSpeed % 10];
	while(1)
	{
		if(cKeyCode)
			DisposeKEY();
	}
}

void Timer0_int(void) interrupt 1 using 1
{
//	TH0 = -9;
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	P0 = 0;
	P2 = wx[point];
	P0 = bit_num[point++];
	point &= 3;

	if(nDelayKey == 0)
	{
		cKey = P1 & 0x1c;	
		if(cKey != 0x1c)
			nDelayKey = 100;
		else
			bStill = 0;
	}
	else		   				
	{
		if(--nDelayKey == 0)
		{
			cKeyCode = P1 & 0x1c;
			if(cKey != cKeyCode)
				cKeyCode = 0;				
		}
	}
	
	if(++nCount == 400)
	{
		TF1 = 0;
		nSpeed = TL1;
		TL1 = 0;
		TH1 = 0;
		nCount = 0;
		bit_num[3] = smg[nSpeed / 10];
		bit_num[2] = smg[nSpeed % 10];
		bit_num[1] = smg[cSetSpeed / 10];
		bit_num[0] = smg[cSetSpeed % 10];
		if(nSpeed < cSetSpeed)
		{
			if (cDuty)
				cDuty--;
		}
		else
		{
			if (cDuty < 99)
				cDuty++;
		}
	}
	if(++cCount == 100)
		cCount = 0;
	if(cDuty < cCount)
			PWM2 = 1;
	else
			PWM2 = 0;
}


