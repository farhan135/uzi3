#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/isr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/pit.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/keyboard.h"

//ctrl or cmd regi: 0x64 8 bit
	//bit 0: output buffer status=0(empty)|1(full)
	//bit 1: input  buffer status=0(empty)|1(full)
//i/o or data regi: 0x60 8 bit

//make code:  key presses or held down
//break code: key is released

uint8_t yCursor;

uint8_t keyboardLayout[167]={
	0,				/*0x00 */
	0,		/*esc		  0x01 */
	'1',				/*0x02 */
	'2',				/*0x03 */
	'3',				/*0x04 */
	'4',				/*0x05 */
	'5',				/*0x06 */
	'6',				/*0x07 */
	'7',				/*0x08 */
	'8',				/*0x09 */
	'9',				/*0x0a */
	'0',				/*0x0b */
	'-',				/*0x0c */
	'=',				/*0x0d */
	'\b',				/*0x0e */
	'\t',				/*0x0f */
	'q',				/*0x10 */
	'w',				/*0x11 */
	'e',				/*0x12 */
	'r',				/*0x13 */
	't',				/*0x14 */
	'y',				/*0x15 */
	'u',				/*0x16 */
	'i',				/*0x17 */
	'o',				/*0x18 */
	'p',				/*0x19 */
	'[',				/*0x1a */
	']',				/*0x1b */
	'\n',				/*0x1c */
	0,		/*ctrl 		  0x1d */
	'a',				/*0x1e */
	's',				/*0x1f */
	'd',				/*0x20 */
	'f',				/*0x21 */
	'g',				/*0x22 */
	'h',				/*0x23 */
	'j',				/*0x24 */
	'k',				/*0x25 */
	'l',				/*0x26 */
	';',				/*0x27 */
	'\\',				/*0x28 */
	'`',		/*check this  	  0x29 */
	0,		/*Lshft       	  0x2a */
	'\\',				/*0x2b */
	'z',				/*0x2c */
	'x',				/*0x2d */
	'c',				/*0x2e */
	'v',				/*0x2f */
	'b',				/*0x30 */
	'n',				/*0x31 */
	'm',				/*0x32 */
	',',				/*0x33 */
	'.',				/*0x34 */
	'/',				/*0x35 */
	0,		/*Rshft 	  0x36 */
	'*',		/*prntScrn	  0x37 */
	0,		/*alt		  0x38 */
	' ',				/*0x39 */
	0,		/*capslock	  0x3a */
	0,		/*f1  		  0x3b */
	0,		/*f2  		  0x3c */
	0,		/*f3  		  0x3d */
	0,		/*f4  		  0x3e */
	0,		/*f5  		  0x3f */
	0,		/*f6  		  0x40 */
	0,		/*f7		  0x41 */
	0,		/*f8  		  0x42 */
	0,		/*f9  		  0x43 */
	0,		/*f10 		  0x44 */
	0,		/*nmlock	  0x45 */
	0,		/*scrlock	  0x46 */
	'7',		/*num home   	  0x47 */
	'8',		/*num upArrow 	  0x48 */
	'9',		/*num pgUp   	  0x49 */
	'-',		/*num         	  0x4a */
	'4',		/*num         	  0x4b */
	'5',		/*num		  0x4c */
	'6',		/*num		  0x4d */
	'+',		/*num		  0x4e */
	'1',		/*num		  0x4f */
	'2',		/*num		  0x50 */
	'3',		/*num		  0x51 */
	'0',		/*num ins	  0x52 */
	'.',		/*num del	  0x53 */
	0,		/*sht f1	  0x54 */
	0,		/*sht f2	  0x55 */
	0,		/*sht f3*/
	0,		/*sht f4*/
	0,		/*sht f5*/
	0,		/*sht f6*/
	0,		/*sht f7*/
	0,		/*sht f8*/
	0,		/*sht f9*/
	0,		/*sht f10*/
	0,		/*ctrl f1*/
	0,		/*ctrl f2*/
	0,		/*ctrl f3*/
	0,		/*ctrl f4*/
	0,		/*ctrl f5 0x5f */
	0,		/*ctrl f6 0x60 */
	0,		/*ctrl f7*/
	0,		/*ctrl f8*/
	0,		/*ctrl f9*/
	0,		/*ctrl f10*/
	0,		/*atl  f1*/
	0,		/*atl  f2*/
	0,		/*atl  f3*/
	0,		/*atl  f4*/
	0,		/*atl  f5*/
	0,		/*atl  f6*/
	0,		/*atl  f7*/
	0,		/*atl  f8*/
	0,		/*atl  f9*/
	0,		/*atl  f10*/
	0,		/*ctrl prntScrn 0x6f */ 
	0,		/*ctrl L arrow  0x70 */
	0,		/*ctrl R arrow*/
	0,		/*ctrl end*/
	0,		/*ctrl pgdn*/
	0,		/*ctrl home*/
	0,		/*atl 1*/
	0,		/*atl 2*/
	0,		/*atl 3*/
	0,		/*atl 4*/
	0,		/*atl 5*/
	0,		/*atl 6*/
	0,		/*atl 7*/
	0,		/*atl 8*/
	0,		/*atl 9*/
	0,		/*atl 0*/
	0,		/*atl -         0x7f */
	0,		/*atl =         0x80 */
	0,		/*ctrl pgup*/
	0,		/*f11*/
	0,		/*f12*/
	0,		/*sht f11*/
	0,		/*sht f12*/
	0,		/*ctrl  f11*/
	0,		/*ctrl  f12*/
	0,		/*atl  f11*/
	0,		/*atl  f12*/
	0,		/*ctrl up arrow*/
	0,		/*ctrl - num*/
	'_',		/*ctrl 5 num     0x8c */
	'+',		/*ctrl + num*/
	0,		/*ctrl dn arrow*/
	0,		/*ctrl ins	 0x8f */
	0,		/*ctrl del       0x90 */
	0,		/*ctrl tab*/
	0,		/*ctrl / num*/
	0,		/*ctrl * num*/
	0,		/*atl home*/
	0,		/*atl up arrow*/
	0,		/*atl pgup*/
	0,		/*nthn*/
	0,		/*atl L arrow*/
	0,		/*nthn*/
	0,		/*atl R arrow*/
	0,		/*nthn*/
	0,		/*atl end*/
	0,		/*atl dn arrow*/
	0,		/*atl pgdn*/
	0,		/*atl ins         0x9f */
	0,		/*atl del         0xa0 */
	0,		/*atl / num       0xa1 */
	0,		/*atl tab         0xa2 */
	0		/*atl enter num   0xa3 */
};

uint8_t readStatus(){
	return inputPort8(0x64);
}

uint8_t getScanCode(){
	uint8_t scanCode;
	while(1) if((readStatus()&0x02)==0) break;
	scanCode=inputPort8(0x60);
return scanCode;
}

void printScanCode(){
	uint8_t scanCode;
	scanCode=getScanCode();
	if(scanCode&0x80);
	else{
		if(yCursor==24){
			if(xCursor==79||scanCode==28)  clearAtScreen(0,10);
		}
		//else{
		if(scanCode==28){
			//if(yCursor==10) puts("root@farhan:/$ ");
			//else puts("\nroot@farhan:/$ ");
			puts("\nroot@farhan:/$ ");
		}
		else putc(keyboardLayout[scanCode]);
		//}
	}
}

void writeIntoOutputBuffer(){
	while(1) if((readStatus()&0x01)==0) break;
}

void enableA20(){
	while(readStatus()&0x02);
	outputPort8(0x64,0xdd);		//enable A20 cmd: 0xdd
	puts("\n\nA20 initialized	[OK]\n");
}

void keyboardCallback(register_t regs){
	
	//if(yCursor==24){
	//	clearAtScreen(0,10);
	//}
	printScanCode();
}

void initKeyboard(){
	uint8_t cmd;
	registerInterruptHandler(IRQ1,&keyboardCallback);
	puts("kbd initialized	[OK]\n");
}
