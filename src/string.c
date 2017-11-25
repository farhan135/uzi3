#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/string.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"

uint8_t strLen(uint8_t *str){
	uint8_t n=0;

	while(*str!='\0'){
		*str++;
		n++;
	}
return n;
}

uint8_t strCmp(uint8_t *str1, uint8_t *str2){
	uint8_t i,n=1;	//true for 0
	uint32_t length1,length2;

	length1=strLen(str1);
	length2=strLen(str2);
	
	if(length1!=length2) n=1;
	else{
		for(i=0;i<length1;i++){
			if(str1[i]!=str2[i]){
				n=1+i;
				break;
			}
			else n=0;
		}
	}
return n;
}

void memSet(uint8_t *destination, uint8_t value, uint32_t length){
	uint8_t *tempt=(uint8_t *)destination;
	for(;length!=0;length--) *tempt++=value;
}
