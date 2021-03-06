%ifndef __GDT_INC_063244045_INCLUDED__
%define __GDT_INC_063244045_INCLUDED__

bits 16

installGDT:
	;cli
	pusha
	xor ax, ax              ; Clear AX register
        mov ds, ax              ; Set DS-register to 0 - used by lgdt
	;call gdt
	lgdt	[gdt_desc]
	;sti
	popa
ret

;----------Global Descriptor Table----------;

gdt:                            ; Address for the GDT

gdt_null:                       ; Null Segment
        dd 0
        dd 0
        
gdt_kernel_code:
        dw 0FFFFh               ; Limit 0xFFFF
        dw 0                    ; Base 0:15
        db 0                    ; Base 16:23
        db 09Ah                 ; Present, Ring 0, Code, Non-conforming, Readable
        db 0CFh                 ; Page-granular
        db 0                    ; Base 24:31

gdt_kernel_data:                        
        dw 0FFFFh               ; Limit 0xFFFF
        dw 0                    ; Base 0:15
        db 0                    ; Base 16:23
        db 092h                 ; Present, Ring 0, Data, Expand-up, Writable
        db 0CFh                 ; Page-granular
        db 0                    ; Base 24:32

gdt_end:                        ; Used to calculate the size of the GDT

gdt_desc:                       ; The GDT descriptor
        dw gdt_end - gdt - 1    ; Limit (size)
        dd gdt                  ; Address of the GDT

%define NULL_DESC 0
%define CODE_DESC 0x8
%define DATA_DESC 0x10

%endif
