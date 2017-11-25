#ifndef _STDIO_H
#define _STDIO_H

#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"


//	index regi: 0x3d5
//	data  regi: 0x3d4

 
//read from port
uint8_t inputPort8(uint16_t _port){
	uint8_t _data;
	__asm__("in %%dx,%%al" : "=a"(_data) : "d"(_port));
	
return _data;
}

uint16_t inputPort16(uint16_t _port){
	uint16_t _data;
	__asm__("in %%dx,%%ax" : "=a"(_data) : "d"(_port));
return _data;
}

//write to port
void outputPort8(uint16_t *_port, uint8_t _data){
	__asm__("out %%al, %%dx" : : "a"(_data), "d"(_port));
}

void outputPort16(uint16_t _port, uint16_t _data){
   __asm__("out %%ax, %%dx" : : "a"(_data), "d"(_port));
}

void ioWait(void){
	//port 0x80 is used for checkpoints during POST, kernel thinks its free to use
	__asm__("out %%al,$0x80"::"a"(0));	//this takes one clock cycle
}
#endif
