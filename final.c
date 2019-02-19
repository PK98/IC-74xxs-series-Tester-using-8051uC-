#include<reg52.h>

#define lcdData P3
#define keyport P0

#define clearDisplay 0x01
#define cursorBlink  0x0E
#define twoLines     0x38
#define start        0x80

sbit RS = P1^4;
sbit RW = P1^5;
sbit E  = P1^6;

void lcdInit(void);
void lcdCmdWrite(unsigned char );
void lcdDataWrite(unsigned char );
void lcdPrint(char *string);
void MSdelay(unsigned int);
void inputkey(void);
void test00(void);
void test02(void);
void test04(void);
void test08(void);
void test32(void);
void test86(void);

unsigned char enter;
unsigned char reset;
unsigned char sum;

void main(void)
{
	while(1){
		enter = 0;
		reset = 0;
		sum = 0;
		lcdInit();
		lcdPrint("WELCOME");
		lcdCmdWrite(0xC0);
		lcdPrint("ENTER IC NO.74");
		while(enter == 0)
		{
			inputkey();
		}
		lcdCmdWrite(clearDisplay);
		lcdPrint("CHECKING IC");
		if(sum == 0)
		{
			test00();
		}
		else if(sum == 2)
		{
			test02();
		}
		else if(sum == 4)
		{
			test04();
		}
		else if(sum == 8)
		{
			test08();
		}
		else if(sum == 5)
		{
			test32();
		}
		else if(sum == 14)
		{
			test86();
		}
		else
		{
			lcdCmdWrite(clearDisplay);
			lcdPrint("IC NOT FOUND");
		} 
		while(reset == 0)
		{
			inputkey();
		}
	}
}

void inputkey(void)
{
	unsigned char check;
	unsigned char pressed = 0;
	keyport = 0x0f;
	check = keyport;
	check &= 0x0f;
	while(check != 0x0f){
		MSdelay(50);
		check = keyport;
		check &= 0x0f;
	}
	MSdelay(5);
	while(pressed == 0){
		check = keyport;
		check &= 0x0f;
	  if(check != 0x0f){
		  MSdelay(5);
		  check = keyport;
			check &= 0xf;
		  if(check != 0x0f){
			  pressed = 1;
		  }
		  else{
			  pressed = 0;
			}
	  }
  }
	
/* checking col 1 */
	keyport = 0xef;
	check = keyport;
	check &= 0x0f;
	if(check != 0x0f){
		if(check == 0x0e){
			lcdPrint("1");
			sum = sum + 1;
		}
		if(check == 0x0d){
			lcdPrint("4");
			sum = sum +4;
		}
		if(check == 0x0b){
			lcdPrint("7");
			sum = sum + 7;
		}
		if(check == 0x07){
			enter = 1;
		}
	}

/* checking col 2 */
	keyport = 0xdf;
	check = keyport;
	check &= 0x0f;
	if(check != 0x0f){
		if(check == 0x0e){
			lcdPrint("2");
			sum = sum + 2;
		}
		if(check == 0x0d){
			lcdPrint("5");
			sum = sum + 5;
		}
		if(check == 0x0b){
			lcdPrint("8");
			sum = sum + 8;
		}
		if(check == 0x07){
			lcdPrint("0");
			sum = sum + 0;
		}
	}	
	
/* checking col 3 */
	keyport = 0xbf;
	check = keyport;
	check &= 0x0f;
	if(check != 0x0f){
		if(check == 0x0e){
			lcdPrint("3");
			sum = sum + 3;
		}
		if(check == 0x0d){
			lcdPrint("6");
			sum = sum + 6;
		}
		if(check == 0x0b){
			lcdPrint("9");
			sum = sum + 9;
		}
		if(check == 0x07){
			reset = 1;
		}
	}	
}

void test00(void)   // NAND gate
{
	unsigned char check;
	lcdCmdWrite(clearDisplay);
	P1 |= 0x0f;
	// case 1
	P2 = 0x00;
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
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

void test02(void)
{
	unsigned char check;
	lcdCmdWrite(clearDisplay);
	P2 = 0xaa;
	//case 1 00
	P2 &= 0xaa;
	P1 &= 0xf0;
	MSdelay(5);
	check = P2;
	check &= 0xaa;
	if(check != 0xaa)
	{
		lcdPrint("BAD");
		return;
	}
	//case 2, 01
	P2 |= 0x55;
	P1 &= 0xf0;
	MSdelay(5);
  check = P2;
	check &= 0xaa;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
	// case 3, 10
	P2 &= 0xaa;
	P1 |= 0x0f;
	MSdelay(5);
	check = P2;
	check &= 0xaa;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
	P2 |= 0x55;
	P1 &= 0xf0;
	MSdelay(5);
	check = P2;
	check &= 0xaa;
	if(check != 0x00)
	{
		lcdPrint("BAD");
		return;
	}
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
		lcdPrint("BAD");
		return;
	}
	lcdPrint("GOOD");
}

void test08(void)    // AND gate
{
	unsigned char check;
	lcdCmdWrite(clearDisplay);
	P1 |= 0x0f; // first 4 bits of P1 as input;
	P2 = 0x00;  // test 1 both 0's
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD 1");
		return;
	}
	P2 = 0x55; // checking 2nd input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD 2");
		return;
	}
	P2 = 0xaa; // checking 3rd input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x00)
	{
		lcdPrint("BAD 3");
		return;
	}
	P2 = 0xff; // checking 4th input
	MSdelay(1);
	check = P1;
	check &= 0x0f;
	if(check != 0x0f)
	{
		lcdPrint("BAD 4");
		return;
	}
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
	lcdCmdWrite(start);
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