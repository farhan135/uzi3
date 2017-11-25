[GLOBAL idtFlush]

idtFlush:
	mov eax,[esp+4]
	lidt[eax]
ret
