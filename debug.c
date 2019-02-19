#include<reg52.h>

void MSdelay(unsigned int);
void main()
{
	while(1)
	{
		P2 = 0x00;
		MSdelay(1000);
		//P2 = 0xFF;
		//MSdelay(1000);
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