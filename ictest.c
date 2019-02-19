#include<reg52.h>

#define lcdData P3

#define clearDisplay 0x01
#define cursorBlink  0x0E
#define twoLines     0x38
#define G1           0x80
#define G2           0x88
#define G3           0xC0
#define G4           0xC8

sbit i1g1 = P2^0;
sbit i2g1 = P2^1;
sbit i1g2 = P2^2;
sbit i2g2 = P2^3;
sbit i1g3 = P2^4;
sbit i2g3 = P2^5;
sbit i1g4 = P2^6;
sbit i2g4 = P2^7;

sbit RS = P1^4;
sbit RW = P1^5;
sbit E  = P1^6;

void lcdInit(void);
void lcdCmdWrite(unsigned char );
void lcdDataWrite(unsigned char );
void lcdPrint(char *string);
void MSdelay(unsigned int);
void test00(void);
void test04(void);
void test08(void);
void test32(void);
void test86(void);

void main(void)
{
	lcdInit();
	lcdPrint("Checking IC");
	MSdelay(1000);
	test00();
	while(1)
	{
	}
}

void test00(void)   // NAND gate
{
	unsigned char check;
	P1 |= 0x0f;
	// case 1
	P2 = 0x00;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	// case 2
	P2 = 0x55;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	// case 3
	P2 = 0xaa;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	// case 4
	P2 = 0xff;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	lcdCmdWrite(clearDisplay);
	lcdPrint("GOOD");
}

void test04(void)     // NOT gate
{
	unsigned char check1,check2;
	lcdCmdWrite(clearDisplay);
	P1 |= 0x05;
	P2 = 0x66;
	// case 1, 1
	P2 |= 0x99;
	P1 |= 0x0a;
	MSdelay(5);
	check1 = P1;
	check1 &= 0x0f;
	check2 = P2;
	check2 &= 0x66;
	if(check1 == 0x00 && check2 == 0x00)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	// case 2, 0
	P2 &= 0x66;
	P1 &= 0x05;
	MSdelay(5);
	check1 = P1;
	check1 &= 0x0f;
	check2 = P2;
	check2 &= 0x66;
	if(check1 == 0x0f && check2 == 0x66)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	lcdCmdWrite(clearDisplay);
	lcdPrint("GOOD");
}

void test08(void)    // AND gate
{
	unsigned char check;
	P1 |= 0x0f; // first 4 bits of P1 as input;
	P2 = 0x00;  // test 1 both 0's
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD");
		return;
	}
	P2 = 0x55; // checking 2nd input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD 2");
		return;
	}
	P2 = 0xaa; // checking 3rd input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD 3");
		return;
	}
	P2 = 0xff; // checking 4th input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdCmdWrite(clearDisplay);
		lcdPrint("BAD 4");
		return;
	}
	lcdCmdWrite(clearDisplay);
	lcdPrint("GOOD");
}

void test32(void)     // OR gate
{
	unsigned char check;
	lcdCmdWrite(clearDisplay);
	P1 |= 0x0f;
	// case 1
	P2 = 0x00;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
	// case 2
	P2 = 0x55;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD");
		return;
	}
	// case 3
	P2 = 0xaa;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD");
		return;
	}
	// case 4
	P2 = 0xff;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD");
		return;
	}
	lcdPrint("GOOD");
}

void test86(void)
{
	unsigned char check;
	lcdCmdWrite(clearDisplay);
	P1 |= 0x0f;
	// case 1
	P2 = 0x00;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
	// case 2
	P2 = 0x55;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD");
		return;
	}
	// case 3
	P2 = 0xaa;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD");
		return;
	}
	// case 4
	P2 = 0xff;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
	lcdPrint("GOOD");
}

void MSdelay(unsigned int time)
{
	unsigned int i,j;
	for(i = 0; i < time; i++)
	{
		for(j=0; j<165; j++);
	}
}

void lcdInit()
{
	lcdCmdWrite(clearDisplay);
	lcdCmdWrite(cursorBlink);
	lcdCmdWrite(twoLines);
	lcdCmdWrite(G1);
}


void lcdCmdWrite(unsigned char command)
{
	RS = 0;
	RW = 0;
	lcdData = command;
	E = 1;
	MSdelay(5);
	E = 0;
	MSdelay(10);
}

void lcdDataWrite(unsigned char Data)
{
	RS = 1;
	RW = 0;
	lcdData = Data;
	E = 1;
	MSdelay(5);
	E = 0;
	MSdelay(10);
}
	
void lcdPrint(char *string)
{
	while(*string > 0)
	{
		lcdDataWrite(*string++);
	}
	
}