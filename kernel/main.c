#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdio.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/descriptorTable.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/pit.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/time.h"

void main(void){
	uint8_t i; // count;
	//uint8_t temp[10];

	setBkgColor(BLUE);
	clearScreen();
	clearAtScreen(0,10);
	setCharColor(BRIGHT_YELLOW);
	putsXY("Booting system",0,1);
	for(i=0;i<50;i++){
		puts(".");
	}
	puts("\n");
	// enableA20();
	initDescriptorTable();
	asm volatile("int $0x1f");	//software interrupt
	asm volatile("sti");
	initTimer(100);
	initKeyboard();
	putsXY("root@farhan:/$ ",0,10);
	
	cli();
	// for(;;);
}

void cli(){
}
