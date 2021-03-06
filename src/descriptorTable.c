#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdgr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/isr.h"
#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/descriptorTable.h"

//gdt var
gdtEntry_t gdtEntries[5];
gdtPtr_t   gdtPtr;

//idt var
idtEntry_t idtEntries[256];
idtPtr_t  idtPtr;

extern isr_t interruptHandler[];

void initGDT(){
	gdtPtr.limit=(sizeof(gdtEntry_t)*5)-1;
	gdtPtr.base=(uint32_t)&gdtEntries;

	gdtSet(0,0,0,0,0);			//null desc
	gdtSet(1,0,0xffffffff,0x9a,0xcf);	//code desc
	gdtSet(2,0,0xffffffff,0x92,0xcf);	//data desc
	gdtSet(3,0,0xffffffff,0xfa,0xcf);	//user code desc
	gdtSet(4,0,0xffffffff,0xf2,0xcf);	//user data desc

	gdtFlush((uint32_t)&gdtPtr);
	
	puts("GDT initialized	[OK]\n");
}

void gdtSet(int32_t indexNum, uint32_t base, uint32_t limit, uint8_t access,uint8_t granularity){
	gdtEntries[indexNum].baseLow=(base&0xffff);
	gdtEntries[indexNum].baseMiddle=(base>>16)&0xff;
	gdtEntries[indexNum].baseHigh=(base>>24)&0xff;

	gdtEntries[indexNum].limitLow=(limit&0xffff);
	gdtEntries[indexNum].granularity=(limit>>16)&0x0f;

	gdtEntries[indexNum].granularity|=granularity&0xf0;
	gdtEntries[indexNum].access=access;
}

void initIDT(){
	idtPtr.limit=sizeof(idtEntry_t)*256-1;
	idtPtr.base=(uint32_t)&idtEntries;

	memSet(&idtEntries,0,(sizeof(idtEntry_t)*256));

	//PIC remapping cz when pc boots default IRQ map is irq0-7 maps to 0x08-0x0f and irq8-15 maps to 0x70-0x77. 
			//but 0x8-0xf conflicts with our software interrupt.Hence the remapping of PIC inorder for hardware remapping. 
	//init cmd
	outputPort8(0x20,0x11);	//command port(M):0x20, command: 0x11 initialization cmd,it tells PIC to wait for 3 extra cmd at data port.
	ioWait();
	outputPort8(0xa0,0x11);
	ioWait();
	//1. set offset
	outputPort8(0x21,0x20); //data port(M):0x21, data:0x20=32
	ioWait();
	outputPort8(0xa1,0x28); //data port(s):0xa1, data:0x28=40. 0x28-0x20=7 bits
	ioWait();
	//2. tell how are they connected or configured
	outputPort8(0x21,0x04); //tells PIC(M) that there is a slave PIC at IRQ2 (0000 00100)
	ioWait();
	outputPort8(0xa1,0x02); //tells PIC(S) that its cascade identity is (0000 00010)
	ioWait();
	//3. provide information about environment
	outputPort8(0x21,0x01);	//8086 mode
	ioWait();
	outputPort8(0xa1,0x01);	//8086 mode
	ioWait();
	//set dummy value or restore saved masks
	outputPort8(0xa1,0x0);
	ioWait();
	outputPort8(0xa1,0x0);
	ioWait();//done

	//idtSet(interruptNum,base,selector,flags);
	idtSet(0,(uint32_t)isr0,0x08,0x8e);
	idtSet(1,(uint32_t)isr1,0x08,0x8e);
	idtSet(2,(uint32_t)isr2,0x08,0x8e);
	idtSet(3,(uint32_t)isr3,0x08,0x8e);
	idtSet(4,(uint32_t)isr4,0x08,0x8e);
	idtSet(5,(uint32_t)isr5,0x08,0x8e);
	idtSet(6,(uint32_t)isr6,0x08,0x8e);
	idtSet(7,(uint32_t)isr7,0x08,0x8e);
	idtSet(8,(uint32_t)isr8,0x08,0x8e);
	idtSet(9,(uint32_t)isr9,0x08,0x8e);
	idtSet(10,(uint32_t)isr10,0x08,0x8e);
	idtSet(11,(uint32_t)isr11,0x08,0x8e);
	idtSet(12,(uint32_t)isr12,0x08,0x8e);
	idtSet(13,(uint32_t)isr13,0x08,0x8e);
	idtSet(14,(uint32_t)isr14,0x08,0x8e);
	idtSet(15,(uint32_t)isr15,0x08,0x8e);
	idtSet(16,(uint32_t)isr16,0x08,0x8e);
	idtSet(17,(uint32_t)isr17,0x08,0x8e);
	idtSet(18,(uint32_t)isr18,0x08,0x8e);
	idtSet(19,(uint32_t)isr19,0x08,0x8e);
	idtSet(20,(uint32_t)isr20,0x08,0x8e);
	idtSet(21,(uint32_t)isr21,0x08,0x8e);
	idtSet(22,(uint32_t)isr22,0x08,0x8e);
	idtSet(23,(uint32_t)isr23,0x08,0x8e);
	idtSet(24,(uint32_t)isr24,0x08,0x8e);
	idtSet(25,(uint32_t)isr25,0x08,0x8e);
	idtSet(26,(uint32_t)isr26,0x08,0x8e);
	idtSet(27,(uint32_t)isr27,0x08,0x8e);
	idtSet(28,(uint32_t)isr28,0x08,0x8e);
	idtSet(29,(uint32_t)isr29,0x08,0x8e);
	idtSet(30,(uint32_t)isr30,0x08,0x8e);
	idtSet(31,(uint32_t)isr31,0x08,0x8e);

	idtSet(32,(uint32_t)irq0,0x08,0x8e);
	idtSet(33,(uint32_t)irq1,0x08,0x8e);
	idtSet(34,(uint32_t)irq2,0x08,0x8e);
	idtSet(35,(uint32_t)irq3,0x08,0x8e);
	idtSet(36,(uint32_t)irq4,0x08,0x8e);
	idtSet(37,(uint32_t)irq5,0x08,0x8e);
	idtSet(38,(uint32_t)irq6,0x08,0x8e);
	idtSet(39,(uint32_t)irq7,0x08,0x8e);
	idtSet(40,(uint32_t)irq8,0x08,0x8e);
	idtSet(41,(uint32_t)irq9,0x08,0x8e);
	idtSet(42,(uint32_t)irq10,0x08,0x8e);
	idtSet(43,(uint32_t)irq11,0x08,0x8e);
	idtSet(44,(uint32_t)irq12,0x08,0x8e);
	idtSet(45,(uint32_t)irq13,0x08,0x8e);
	idtSet(46,(uint32_t)irq14,0x08,0x8e);
	idtSet(47,(uint32_t)irq15,0x08,0x8e);
	
	idtFlush((uint32_t)&idtPtr);

	puts("IDT initialized	[OK]\n");
}

void idtSet(uint8_t indexNum,uint32_t base, uint16_t selector, uint8_t flags){
	idtEntries[indexNum].baseLow=base&0xffff;
	idtEntries[indexNum].baseHigh=(base>>16)&0xffff;

	idtEntries[indexNum].selector=selector;
	idtEntries[indexNum].reserved=0;
	//uncomment to achieve user mode
	idtEntries[indexNum].flags=flags; /*|0x60*/
}

void initDescriptorTable(){
	initGDT();
	initIDT();

	memSet(&interruptHandler,0,sizeof(isr_t)*256);
}
