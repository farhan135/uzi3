bits	16						; we are in 16 bit real mode
org	0						; we will set regisers later
start:	jmp	main					; jump to start of bootloader
;----------------------------------------------------------------------------------------------
%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/filesystem.inc"
%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/stdio.inc"
;----------------------------------------------------------------------------------------------
main:

	; code located at 0000:7C00, Adjust Segment registers
	cli					; disable interrupts
	mov ax, 0x07C0				; setup registers to point to our segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Setup stack
	mov ax, 0x0000				; set the stack
	mov ss, ax
	mov sp, 0xFFFF
	sti					; restore interrupts
	
	; Display loading message     
;	mov si, msgLoading
;	call print

	call kernelLoader
				 
;msgLoading	db	0x0D, 0x0A, "Booting", 0x00
msgCRLF		db	0x0D, 0x0A, 0x00
msgProgress	db	".", 0x00
msgFailure	db	0x0D, 0x0A, "Error", 0x0A, 0x00
     
TIMES 510-($-$$) DB 0
DW 0xAA55
