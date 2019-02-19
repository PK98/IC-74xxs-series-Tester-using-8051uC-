#include<reg52.h>

sbit led = P2^1;
sbit led2 = P0^0;

void MSdelay(unsigned int);
void main(void)
{
	P0 = 0x0f;
	while(1)
	{
		led = 0;
		led2 = 0;
		MSdelay(1000);
		led = 1;
		led2 = 1;
		MSdelay(1000);
	}
}

void MSdelay(unsigned int time)
{
	unsigned int i,j;
	for(i = 0; i < time; i++)
	{
		for(j=0; j<165; j++);
	}
}