
.model tiny
.code
org 100h

Start: 		mov si, offset Message		;dx = &Message		

		mov ah, 0eh			;function = print as tty
		mov cx, 01h			;print count = 0
		mov bh, 00h			;screen block = 0

		mov bl, 00h			;ending character
	
Print:		mov al, [si]			;al = Message[i]
		cmp al, bl			;if al == ending_char
		je Exit				;break
		inc si				;i++
		int 10h				;putc(Message[i])
		jmp Print 			
		
Exit:		mov ax, 4c00h
		int 21h


Message:	db "The world is gonna roll me fuck you", 0

end		Start