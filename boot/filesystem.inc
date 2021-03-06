%ifndef __FILESYSTEM_INC_063244045_INCLUDED__
%define __FILESYSTEM_INC_063244045_INCLUDED__

;	BIOS Parameter Block
;-------------------------------------------------------------------------------------------------
bpbOEM			db "My OS   "			; OEM identifier (Cannot exceed 8 bytes!)
bpbBytesPerSector  	dw 512
bpbSectorsPerCluster 	db 1
bpbReservedSectors 	dw 1
bpbNumberOfFATs 	db 2
bpbRootEntries	 	dw 224
bpbTotalSectors 	dw 2880
;bpbTotalSectors 	dw 17280
bpbMedia 		db 0xf0  ;; 0xF1
bpbSectorsPerFAT 	dw 9
bpbSectorsPerTrack 	dw 18
bpbNumOfHeads		dw 2
bpbHiddenSectors 	dd 0
bpbTotalSectorsBig	dd 0
bpbDriveNumber		db 0
bsUnused 		db 0
bsExtBootSignature 	db 0x29
bsSerialNumber	        dd 0xa0a1a2a3
bsVolumeLabel 	        db "myOS_UZI   "
bsFileSystem 	        db "FAT12   "

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

	;-------------------------------------------------------------------------------------
	; Load root directory table
	loadRoot:
		; compute size of root directory and store it in "cx". Then copy it to ax to 
		xor cx, cx
		xor dx, dx
		mov ax, 0x0020                          ; 32 byte directory entry
		mul word[bpbRootEntries]                ; total size of directory
		div word[bpbBytesPerSector]             ; sectors used by directory
		xchg ax, cx				; cx = sizeof(root dir)

		; compute location of root directory and store in "ax"
     		xor ax, ax
		mov al, byte[bpbNumberOfFATs]            ; number of FATs
		mul word[bpbSectorsPerFAT]               ; sectors used by FATs
		add ax, word[bpbReservedSectors]         ; adjust for bootsector
		mov word[dataSector], ax                 ; base of root directory
		add word[dataSector], cx

		; ax = base of root dir
		; cx = sizeof root dir
		; dataSector = total sector to load from root dir. 
		; This will be added to the clusterToLBA value when loading stage2 image file.

		; read root directory into memory (07C0:0200)
		push word[rootSegment]
		pop es
		mov bx, 0x0200                            ; copy root dir above bootcode
		call readSectors
	ret
	;-------------------------------------------------------------------------------------
;-------------------------------------------------------------------------------------
	; Find stage 2
	; browse root directory for binary image
	; ret: if foundstage2 file cluster then load loadFAT table
	;	else stage2 file is not found then jmp to failure message.
	findImage:
		mov cx, word[bpbRootEntries]             ; load loop counter
		mov di, 0x0200                            ; locate first root entry		
		.loopHere:
			cld
			push cx
			mov cx, 0x000B                    ; eleven character name
			; ds:si
			mov si, imageName                 ; image name to find
			; es:di
			push di
			repe cmpsb                       ; test for entry match			
			pop di
			pop cx
		;ret: di has the starting cluster of stage2 image
			je returnProc
			add di, 0x0020                    ; queue next directory entry		
			loop .loopHere
		; ret:
			jmp failure
		returnProc:
			; 0x001c is 28th bit, of 32 byte root directory entry.
			;mov ax, word[es:di+0x001c]	; [di]=0x524b=KR, 4c4e=NL, 5244=DR, 2020=" ", 5953=SY, 2053=S| 1f=4100h=16640, 
							; 20=6b41h=
			;call printAX 
			ret
	;----------------------------------------------------------------------------
	; load FAT
	loadFAT:
		; es=0x07c0, di=0x0220
		; save starting cluster of boot image
		mov ax, word[es:di+0x001a]		; problem is here, the clusterValue is 0x0000. 
		;call printAX		
		mov word[cluster], ax                   ; file's first cluster	
		; compute sizeof FAT and store in "cx"
		xor ax, ax
		mov al, byte[bpbNumberOfFATs]           ; number of FATs
		mul word[bpbSectorsPerFAT]              ; sectors used by FATs
		mov cx, ax

		; compute location of FAT and store in "ax"
		mov ax, word[bpbReservedSectors]        ; adjust for bootsector
		; read FAT into memory (07C0:0200)
		push word[fatSegment]
		pop es
		mov bx, 0x0200                          ; copy FAT above bootcode
		call readSectors
	ret
	;----------------------------------------------------------------------------
	; Load Stage2 file
	loadImage:
		mov ax, word[cluster]                    ; cluster to read	
		call printAX	
		call clusterLBA                          ; convert cluster to LBA
		xor cx, cx
		mov cl, byte[bpbSectorsPerCluster]       ; sectors to read
		call readSectors
		push bx					 ; saving bx at here so that at line 238 when it pops, gets back previous value.
          
		; compute next cluster
		xor ax, ax
		inc ax
		add byte[imageSize], al
		mov ax, word[cluster]		; identify current cluster
		mov cx, ax				; copy current cluster
		mov dx, ax				; copy current cluster
		shr dx, 0x0001			; divide by two. we divide by 2 to find even or odd cluster.
		add cx, dx				; sum for (3/2)
		mov bx, 0x0200			; location of FAT in memory
		add bx, cx				; index into FAT
		mov dx, word[bx]			; read two bytes from FAT

		test ax, 0x0001			; test performs an bitwise AND operation on two operands.
		jnz  .oddCluster

		.evenCluster:
			and dx, 0000111111111111b               ; take low twelve bits
			jmp .done

		.oddCluster:
			shr dx, 0x0004                          ; take high twelve bits
			
		.done:
			pop bx					; to balance stack from breaking at line 242
			mov word[cluster],dx			; store new cluster							
			cmp dx, 0x0FF0				; test for end of file. 0x0ff0 is the last file cluster.
			jb loadImage
			mov si, msgCRLF
			call print			
			xor ax, ax
			mov al, byte[imageSize]
			call printAX
			;cli
			;hlt
			; we make a far jmp to 0x0050:0x0000		
;			push word 0x0050
;			push word 0x0000		
;			retf	
;--------------------------------------------------------------------------------------------------------		
failure:
	mov     si, msgFailure
	call    print
	mov     ah, 0x00
	int     0x16                                ; await keypress
	int     0x19                                ; warm boot computer

;========================================================================================================================================
kernelLoader:
	mov word[rootSegment], 0x07c0
	call loadRoot

	call findImage

	mov word[fatSegment], 0x07c0
	call loadFAT

	; read image file into memory (0050:0000)
	mov bx, 0x0050
	mov es, bx                              ; destination for image
	xor bx, bx                          ; destination for image
	call loadImage
	
;-----------------------------------------------------
absoluteSector	db	0x00
absoluteHead	db	0x00
absoluteTrack	db	0x00
;--------------
imageName	db	"KRNLDR  SYS"	; 11 bits
imageSize	db	0x00
;--------------
rootSegment	dw	0x0000
fatSegment	dw	0x0000
imageSegment	dw	0x0000
;--------------
dataSector	dw	0x0000
cluster		dw	0x0000
;-----------------------------------------------------
%endif
