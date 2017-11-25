#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/isr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"

isr_t interruptHandler[256];

void isrHandler(register_t regs){
	puts("\nRecieved interrupt: ");
	puts_dec(regs.interruptNum);
	puts("\n");
	if(interruptHandler[regs.interruptNum]!=0){
		isr_t handler;
		handler=interruptHandler[regs.interruptNum];
		handler(regs);
	}
}

void irqHandler(register_t regs){
	//send eoi signal to PIC
	if(regs.interruptNum>=40){
		//reset signal to slave
		outputPort8(0xa0,0x20);
	}
	//reset signal to master
	outputPort8(0x20,0x20);

	//if interrupt present
	if(interruptHandler[regs.interruptNum]!=0){
		isr_t handler=interruptHandler[regs.interruptNum];
		handler(regs);	//calling upon service routine function
	}
}

void registerInterruptHandler(uint8_t indexNum,isr_t handler){
	interruptHandler[indexNum]=handler;
}
