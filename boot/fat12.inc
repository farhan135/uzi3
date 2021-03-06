%ifndef __FAT12_INC_063244045_INCLUDED__
%define __FAT12_INC_063244045_INCLUDED__

bits	16

%define ROOT_OFFSET 0x2e00
%define FAT_SEG 0x2c0
%define ROOT_SEG 0x2e0
%define IMAGE_PMODE_BASE 0x100000	; where the kernel is to be loaded to in protected mode
%define IMAGE_RMODE_BASE 0x3000		; where the kernel is to be loaded to in real mode

ImageSize     db 0			; size of kernel image in bytes

absoluteSector	db	0x00
absoluteHead	db	0x00
absoluteTrack	db	0x00

dataSector	dw	0x0000
cluster		dw	0x0000

;	BIOS Parameter Block
;-------------------------------------------------------------------------------------------------
;bpbOEM			db "My OS   "			; OEM identifier (Cannot exceed 8 bytes!)
bpbBytesPerSector  	dw 512
bpbSectorsPerCluster 	db 1
bpbReservedSectors 	dw 1
bpbNumberOfFATs 	db 2
bpbRootEntries	 	dw 224
;bpbTotalSectors 	dw 2880
;bpbMedia 		db 0xf8  ;; 0xF1
bpbSectorsPerFAT 	dw 9
bpbSectorsPerTrack 	dw 18
bpbNumOfHeads		dw 2
;bpbHiddenSectors 	dd 0
;bpbTotalSectorsBig	dd 0
bpbDriveNumber		db 0
;bsUnused 		db 0
;bsExtBootSignature 	db 0x29
;bsSerialNumber	        dd 0xa0a1a2a3
;bsVolumeLabel 	        db "myOS_UZI   "
;bsFileSystem 	        db "FAT12   "

;-----------------------------------------------------------------
;INT 0x13/AH=0x02 - DISK : READ SECTOR(S) INTO MEMORY
;AH = 0x02
;AL = Number of sectors to read
;CH = Low eight bits of cylinder number
;CL = Sector Number (Bits 0-5). Bits 6-7 are for hard disks only
;DH = Head Number
;DL = Drive Number (Bit 7 set for hard disks)
;ES:BX = Buffer to read sectors to
;===================================
;Returns:
;AH = Status Code
;AL = Number of sectors read
;CF = set if failure, cleared is successfull
; readSectors(@es:bx, @ax): 
;-----------------------------------------------------------------
readSectors:
	.mainLoop:
		mov	di, 0x0005                          ; five retries for error
		.sectorLoop:
			push ax
			push bx
			push cx
			call LBAtoCHS                              ; convert starting sector to CHS
			mov  ah,0x02                            ; BIOS read sector
			mov  al,0x01                            ; read one sector
			mov  ch,BYTE [absoluteTrack]            ; track
			mov  cl,BYTE [absoluteSector]           ; sector
			mov  dh,BYTE [absoluteHead]             ; head
			mov  dl,BYTE [bpbDriveNumber]            ; drive
			int  0x13                                ; invoke BIOS
			jnc  .success                            ; test for read error
			xor  ax,ax                              ; BIOS reset disk
			int  0x13                                ; invoke BIOS
			dec  di                                  ; decrement error counter
			pop  cx
			pop  bx
			pop  ax
			jnz  .sectorLoop                         ; attempt to read again
			int  0x18
		.success:
			mov  si,msgProgress
			call print
			pop  cx
			pop  bx
			pop  ax
			add  bx, WORD[bpbBytesPerSector]		; queue next buffer
			inc  ax					; queue next sector
			loop .mainLoop				; read next sector
ret
;-------------------------------------------------------------------------
; sector is 512 bytes.
; Track is a circle can be divided into sectors.
; Cylinder is group of tracks withb fixed radius.
; Two heads for each track.
; es:bx is buffer
; clusterLBA(@ax): return ax
;-------------------------------------------------------------------------
clusterLBA:
	; input ax= cluster
	; lba= (cluster-2)*sectorsPerCluster
	sub ax,0x0002                          ; zero base cluster number
	xor cx,cx
	mov cl,BYTE [bpbSectorsPerCluster]     ; convert byte to word
	mul cx
	add ax,WORD [dataSector]               ; base data sector
ret
;-----------------------------------------------------------------
; Note:
; div: divides ax by operand. outputs ax=quotient, dx=remainder.
; module(%): gives the remainder.
; mul: multiplies ax by operand. Output is ax:cx.
;-----------------------------------------------------------------
LBAtoCHS:
	; input param: ax= lba value = calculated cluster location
	;-------------------------------------------
	; output:
	; absoluteSector=(lba%sectorPerTrack)+(1)
	; absoluteTrack/cylinder=(lba/sectorPerTrack)/(numOfHeads)
	; absoluteHead=(lba/sectorPerTrack)%(numOfHeads)
	;------------------------------------------- 
	;=================================================================
 	  xor dx, dx                              ; prepare dx:ax for operation
          div word[bpbSectorsPerTrack]           ; calculate
          inc dl                                  ; adjust for sector 0
          ; sector:
		mov byte[absoluteSector], dl
	  ;---------------------------------
	  xor dx, dx                              ; prepare dx:ax for operation
	  div word[bpbNumOfHeads]          	    ; calculate
	  ; cylinder:
		mov byte[absoluteTrack], al
	  ;---------------------------------
          ; head:
	  	mov byte[absoluteHead], dl
	  ;---------------------------------
	;=================================================================	
ret
;----------------------------------------------------------------------------------------------


;*******************************************
; LoadRoot ()
;	- Load Root Directory Table to 0x7e00
;*******************************************

LoadRoot:
	pusha							; store registers
	push	es
     ; compute size of root directory and store in "cx"
	xor     cx, cx						; clear registers
 	xor     dx, dx
	mov     ax, 32					; 32 byte directory entry
	mul     WORD [bpbRootEntries]				; total size of directory
	div     WORD [bpbBytesPerSector]			; sectors used by directory
	xchg    ax, cx						; move into AX
     ; compute location of root directory and store in "ax"
	mov     al, BYTE [bpbNumberOfFATs]			; number of FATs
	mul     WORD [bpbSectorsPerFAT]				; sectors used by FATs
	add     ax, WORD [bpbReservedSectors]
	mov     WORD [dataSector], ax				; base of root directory
	add     WORD [dataSector], cx
     ; read root directory into 0x7e00
 	push	word ROOT_SEG
	pop		es
	mov     bx, 0								; copy root dir
	call    readSectors							; read in directory table
	pop		es
	popa										; restore registers and return
	ret
;*******************************************
; LoadFAT ()
;	- Loads FAT table to 0x7c00
;
;	Parm/ ES:DI => Root Directory Table
;*******************************************

LoadFAT:
	pusha							; store registers
	push	es
     ; compute size of FAT and store in "cx"
	xor     ax, ax
	mov     al, BYTE [bpbNumberOfFATs]			; number of FATs
	mul     WORD [bpbSectorsPerFAT]				; sectors used by FATs
	mov     cx, ax
     ; compute location of FAT and store in "ax"
	mov     ax, WORD [bpbReservedSectors]
     ; read FAT into memory (Overwrite our bootloader at 0x7c00)
	push	word FAT_SEG
	pop		es
	xor		bx, bx
	call    readSectors
	pop		es
	popa							; restore registers and return
	ret
	
;*******************************************
; FindFile ()
;	- Search for filename in root table
;
; parm/ DS:SI => File name
; ret/ AX => File index number in directory table. -1 if error
;*******************************************

FindFile:

	push	cx						; store registers
	push	dx
	push	bx
	mov	bx, si						; copy filename for later

     ; browse root directory for binary image

	mov     cx, WORD [bpbRootEntries]			; load loop counter
	mov     di, ROOT_OFFSET						; locate first root entry at 1 MB mark
	cld							; clear direction flag

.LOOP:
	push    cx
	mov     cx, 11					; eleven character name. Image name is in SI
	mov	si, bx						; image name is in BX
 	push    di
     repe  cmpsb							; test for entry match
	pop     di
	je      .Found
	pop     cx
	add     di, 32					; queue next directory entry
	loop    .LOOP

.NotFound:
	pop	bx						; restore registers and return
	pop	dx
	pop	cx
	mov	ax, -1						; set error code
	ret

.Found:
	pop	ax						; return value into AX contains entry of file
	pop	bx						; restore registers and return
	pop	dx
	pop	cx
	ret

;*******************************************
; LoadFile ()
;	- Load file
; parm/ ES:SI => File to load
; parm/ EBX:BP => Buffer to load file to
; ret/ AX => -1 on error, 0 on success
; ret/ CX => number of sectors read
;*******************************************

LoadFile:

	xor	ecx, ecx		; size of file in sectors
	push	ecx

.FIND_FILE:

	push	bx			; BX=>BP points to buffer to write to; store it for later
	push	bp
	call	FindFile		; find our file. ES:SI contains our filename
	cmp	ax, -1
	jne	.LOAD_IMAGE_PRE
	pop	bp
	pop	bx
	pop	ecx
	mov	ax, -1
	ret

.LOAD_IMAGE_PRE:

	sub	edi, ROOT_OFFSET
	sub	eax, ROOT_OFFSET

	; get starting cluster

	push	word ROOT_SEG		; root segment loc
	pop	es
	mov	dx, WORD [es:di + 0x001A]; DI points to file entry in root directory table. Refrence the table...
	mov	WORD [cluster], dx	; file's first cluster
	pop	bx			; get location to write to so we dont screw up the stack
	pop	es
	push    bx			; store location for later again
	push	es
	call	LoadFAT

.LOAD_IMAGE:

	; load the cluster

	mov	ax, WORD [cluster]	; cluster to read
	pop	es			; bx:bp=es:bx
	pop	bx
	call	clusterLBA
	xor	cx, cx
	mov     cl, BYTE [bpbSectorsPerCluster]
	call	readSectors
	pop	ecx
	inc	ecx			; add one more sector to counter
	push	ecx
	push	bx
	push	es
	mov	ax, FAT_SEG		;start reading from fat
	mov	es, ax
	xor	bx, bx

	; get next cluster

	mov     ax, WORD [cluster]	; identify current cluster
	mov     cx, ax			; copy current cluster
	mov     dx, ax
	shr     dx, 0x0001		; divide by two
	add     cx, dx			; sum for (3/2)

	mov	bx, 0			;location of fat in memory
	add	bx, cx
	mov	dx, WORD [es:bx]
	test	ax, 0x0001		; test for odd or even cluster
	jnz	.ODD_CLUSTER

.EVEN_CLUSTER:

	and	dx, 0000111111111111b	; take low 12 bits
	jmp	.DONE

.ODD_CLUSTER:

	shr	dx, 0x0004		; take high 12 bits

.DONE:

	mov	WORD [cluster], dx
	cmp	dx, 0x0ff0		; test for end of file marker
	jb	.LOAD_IMAGE

.SUCCESS:
	pop	es
	pop	bx
	pop	ecx
	xor	ax, ax
	ret

%endif		;__FAT12B_INC_063244045_INCLUDED__
