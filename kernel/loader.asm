
bits 32

extern main
global start

jmp start

;%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/stdio.inc"

start:

	call main	;call kernel's main

	cli		;clear int
	hlt		;hlt system

msg	db	0x0D, 0x0A, "debug:>", 0x00
