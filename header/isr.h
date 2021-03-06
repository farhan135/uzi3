#ifndef _ISR_H
#define _ISR_H

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"

typedef struct registers{
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t interruptNum,errCode;
	uint32_t eip, cs, eflags, useresp, ss;
}register_t;

void isrHandler(register_t);
void irqHandler(register_t);
//isr_t is funcpointer type
typedef void (*isr_t)(register_t);
void registerInterruptHandler(uint8_t n, isr_t handler);

#endif
