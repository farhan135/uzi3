#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/isr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/pit.h"

//uint32_t 
uint32_t tick=0;
uint32_t tickInSecond=0;

void timerCallback(register_t regs){
	tick++;
	if((tick%18)==0) tickInSecond++;
	//putsXY("\nClock tick: ",2,10);
	//puts_dec(tickInSecond);
}

void initTimer(uint32_t frequency){

	uint32_t divisor;
	registerInterruptHandler(IRQ0,&timerCallback);

	//send value to PIT, to slow it down
	divisor=1193180/frequency;
 
	outputPort8(0x43,0x36);	//send cmd byte

	//send divisor byte wise
	outputPort8(0x40,(uint8_t)divisor&0xff);
	outputPort8(0x40,(uint8_t)(divisor>>8)&0xff);
	puts("PIT initialized	[OK]\n");
}
