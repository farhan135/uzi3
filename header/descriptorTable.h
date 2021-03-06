#ifndef _STDDESCRIPTORTABLE_H
#define _STDDESCRIPTORTABLE_H
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"

struct gdtEntry{
	uint16_t limitLow;
	uint16_t baseLow;
	uint8_t  baseMiddle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  baseHigh;
}__attribute__((packed));

typedef struct gdtEntry gdtEntry_t;

struct gdtPtr{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

typedef struct gdtPtr gdtPtr_t;

//gdtFlush is in asm
extern void gdtFlush(uint32_t);

void initGDT();
void initDescriptorTable();
void gdtSet(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);

//IDT desc here:

struct idtEntry{
	uint16_t baseLow;
	uint16_t selector;
	uint8_t  reserved;
	uint8_t  flags;
	uint16_t baseHigh;
}__attribute__((packed));

typedef struct idtEntry idtEntry_t;

struct idtPtr{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

typedef struct idtPtr idtPtr_t;

extern void idtFlush(uint32_t);

void initIDT();
void idtSet(uint8_t, uint32_t, uint16_t, uint8_t);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
