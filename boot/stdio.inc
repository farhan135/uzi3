%ifndef __STDIO_INC_063244045_INCLUDED__
%define __STDIO_INC_063244045_INCLUDED__

bits 16

;--------------------------------------------------------------------------------------
print:
	lodsb				; load next byte from string from SI to AL
	or	al, al			; Does AL=0?
	jz	printDone		; Yep, null terminator found-bail out
	mov	ah, 0eh			; Nope-Print the character
	int	10h
	jmp	print			; Repeat until null terminator found
printDone:
	ret				; we are done, so return
;--------------------------------------------------------------------------------------
printAX:
	push cx
	mov cx, 4
next_digit:
	push cx
	mov cl, 4
	rol ax, cl
	push ax
	and al, 0x0f
	add al, '0'
	cmp al, '9'
	jle not_a_letter
	add al, 'A'-'9'-1
not_a_letter:
  ; Now write the character AL to the screen
  	mov ah, 0x0e ; teletype sub-function.
	int 0x10
	pop ax
	pop cx
	loop next_digit
	pop cx
ret
;--------------------------------------------------------------------------------------


%endif
