#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/string.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"

uint16_t charAttribute;
uint8_t xCursor=0,yCursor=0;
uint16_t *vidMem = (uint16_t *)0xB8000; 		/* pointer to video memory */

//background color
void setBkgColor(uint8_t color){
	charAttribute=0x0000;
	charAttribute|=color<<12;
}

//foreground color
void setCharColor(uint8_t color){
	charAttribute|=color<<8;
}

void gotoXY(uint8_t x,uint8_t y){
	if(xCursor<=80) xCursor=x;
	if(yCursor<=25) yCursor=y;
}

void print(uint8_t ch,uint8_t i){
	uint16_t *pos;

	pos=vidMem+(yCursor*SCREEN_WIDTH+xCursor);	//offset: yCursor*SCREEN_WIDTH+xCursor relative to vidMem(base address)
	*pos=ch|charAttribute;
	//if i==0 increment xCursor else dnt
	if(i==0){
		xCursor++;
		if(xCursor>=80){
			xCursor=0;
			yCursor++;
		}
	}
	updateCursor(yCursor,xCursor);
}

void clearScreen(){
	gotoXY(0,0);
	int i;
	for(i=0;i<SCREEN_MAX_CHAR;++i){		
    		print(' ',0);
	}
	gotoXY(0,0);
}

void clearAtScreen(uint8_t x, uint8_t y){
	gotoXY(x,y);
	int i;
	for(i=0;i<SCREEN_MAX_CHAR-(y*80);++i){		
    		print(' ',0);
	}
	gotoXY(x,y);
}

uint8_t checkTabPressed(uint16_t *pos1){
	uint8_t temp[8];
	uint16_t currentLoc, i;			
	memSet(temp,'\0',8);
	currentLoc=xCursor-8;
	i=0;
	while(currentLoc<xCursor){
		pos1=vidMem+(yCursor*SCREEN_WIDTH+currentLoc);
		if((temp[i]=*pos1)==' '){
			i++;
			currentLoc++;
		}
		else break;
	}
return i;
}

void putc(uint8_t ch){
	uint8_t i,currentLoc=0;
	uint16_t *pos;
	uint8_t temp[16];
	if(ch=='\n'){
		if(yCursor>=24) yCursor-=14;
		else yCursor++;
		xCursor=0;
		updateCursor(yCursor,xCursor);
	}
	else if(ch=='\t'){ xCursor+=8; updateCursor(yCursor,xCursor);}
	else if(ch=='\b'){
		//checkBoundary(){
			for(i=0;i<15;i++){
				pos=vidMem+(yCursor*SCREEN_WIDTH+i);
				temp[i]=*pos;
			}
			temp[i]='\0';
			if((i=strCmp(temp,"root@farhan:/$ \0"))==0){
				if(xCursor>15){
					if(checkTabPressed(pos)==8) xCursor-=8;
					else xCursor--;
					print(' ',1);
				}
			}
			else{
				if(xCursor==0){ 
					xCursor=80;
					yCursor--;
					updateCursor(yCursor,xCursor);
					xCursor--;
					print(' ',1);
				}
				else{
					xCursor--;
					print(' ',1);
				}
			}
		//return checkBoundary;
	}
	else if(ch>=' '){
		print(ch,0);
	}
	/**else if(yCursor>24){
		for(i=0;i<24*80;i++){
			vidMem[i]=vidMem[i+80];
		}
		vidMem[24*80]=' ';
		yCursor=24;
	}**/
	
}

void puts(char *msg){
	while(*msg) putc(*msg++);
}

void updateCursor(uint8_t y,uint8_t x){
	uint16_t  location;
	uint16_t  port;
	uint8_t   data;

	location=(x+y*SCREEN_WIDTH);
//set current cursor position
	port=0x03D4;
	data=0x0f;
	outputPort8(port,data);
	port=0x03D5;
	data=(uint8_t)location;
	outputPort8(port,data);
	
	port=0x03D4;
	data=0x0e;
	outputPort8(port,data);
	port=0x03D5;
	data=(uint8_t)(location>>8);
	outputPort8(port,data);
}

void putcXY(uint8_t ch,uint8_t x,uint8_t y){
	gotoXY(x,y);
	putc(ch);
}

void putsXY(char *msg,uint8_t x,uint8_t y){
	gotoXY(x,y);
	puts(msg);
}

void puts_hex(uint32_t n){
	int32_t tmp;
	int32_t i;
	uint8_t noZeroes = 1;

	puts("0x");


	for (i = 28; i > 0; i -= 4){
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0){
			continue;
		}
        	else if (tmp >= 0xA){
			noZeroes = 0;
			putc(tmp-0xA+'a' );
        	}
        	else{
			noZeroes = 0;
			putc( tmp+'0' );
		}
	}
  
	tmp=n&0xF;
	if (tmp>=0xA){
		putc(tmp-0xA+'a');
	}
	else putc(tmp+'0');
}

void puts_dec(uint32_t n)
{
	int32_t acc=n,i=0,j=0;
	uint8_t c[32];
	uint8_t c2[32];

	if (n == 0){
		putc('0');
		return;
	}

	while (acc > 0){
		c[i] = '0' + acc%10;
		acc /= 10;
		i++;
	}
	c[i] = 0;
	c2[i--] = 0;

	while(i >= 0){
		c2[i--] = c[j++];
	}
	puts(c2);
}
