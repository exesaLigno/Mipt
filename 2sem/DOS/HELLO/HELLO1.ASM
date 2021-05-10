
.model tiny
.code
org 100h

Start:		mov dx, offset Message

		nop
		nop
		nop

		mov ah, 09h
		int 21h

		mov ax, 4c00h
		int 21h

Message: 	db "Hello World!", 13, 10, "$"

end 		Start