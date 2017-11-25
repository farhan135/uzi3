#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/pit.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"

uint32_t tickInSecond;

void delay(uint32_t timeInSecond){
	uint32_t t2;

	t2=tickInSecond+timeInSecond;
	while(tickInSecond<t2);
}
