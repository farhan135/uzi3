bits 16                       ; We need 16-bit intructions for Real mode
org 0x0500	                      ; The BIOS loads the boot sector into memory location 0x0500

jmp enter_pm   	        	; Load the krnldr

%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/gdt.inc"
%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/a20.inc"
%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/stdio.inc"
%include "/home/farhan/Desktop/myOS_UZI/uzi3/boot/fat12.inc"

enter_pm:
        cli
	xor ax, ax  		; null segments	
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, 0x0000          ; stack begins at 0x9000-0xffff
	mov ss, ax
	mov sp, 0xffff 

	mov  si,msgLoading
	call print
	
	call enableA20
	call installGDT

	call LoadRoot

        ; Load Kernel
        ;-------------------------------
	mov	ebx, 0			; BX:BP points to buffer to load to
   	mov	bp, IMAGE_RMODE_BASE
	mov	si, ImageName		; our file to load

	call	LoadFile		; load our file
	mov	dword [ImageSize], ecx	; save size of kernel
	mov eax, ecx
	call printAX
	cli
	hlt
	;
	cmp	ax, 0			; Test for success
	je	EnterStage3		; yep--onto Stage 3!
	mov	si, msgFailure		; Nope--print error
	call	print
	mov	ah, 0
	int     0x16                    ; await keypress
	int     0x19                    ; warm boot computer
	cli				; If we get here, something really went wong
	hlt

	;-------------------------------;
	;   Go into pmode		;
	;-------------------------------;

EnterStage3:

	cli				; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax

	jmp	CODE_DESC:Stage3	; far jump to fix CS. Remember that the code selector is 0x8!

	; Note: Do NOT re-enable interrupts! Doing so will triple fault!
	; We will fix this in Stage 3.

;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************

bits 32

Stage3:

	;-------------------------------
	;   Set registers		
	;-------------------------------

	mov	ax, DATA_DESC	; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h

	; Copy kernel to 1MB		
	;-------------------------------

CopyImage:
  	 mov	eax, dword [ImageSize]
  	 movzx	ebx, word [bpbBytesPerSector]
  	 mul	ebx
  	 mov	ebx, 4
  	 div	ebx
   	 cld
   	 mov    esi, IMAGE_RMODE_BASE
   	 mov	edi, IMAGE_PMODE_BASE
   	 mov	ecx, eax
   	 rep	movsd                   ; copy image to its protected mode address

	;   Execute Kernel			
	;---------------------------------------

	jmp	CODE_DESC:IMAGE_PMODE_BASE; jump to our kernel! Note: This assumes Kernel's entry point is at 1 MB

	cli
	hlt

msg		db	0x0D, 0x0A, "debug:>", 0x0d, 0x0a, 0x00
msgProgress	db	".", 0x00
msgLoading	db	0x0D, 0x0A, "krnldr.sys loaded successfully.", 0x0D, 0x0A, 0x00
msgFailure	db 	0x0D, 0x0A, "*** FATAL: MISSING OR CURRUPT KRNL.SYS. Press Any Key to Reboot", 0x0D, 0x0A, 0x00
ImageName	db	"KERNEL  SYS"		; Must be 11 bytes
