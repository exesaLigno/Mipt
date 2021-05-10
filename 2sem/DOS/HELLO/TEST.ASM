

.model tiny
.code
org 100h

Start:		mov ah, 09h
		mov dx, offset Message
		int 21h

		mov ax, 4c00h
		int 21h


Message:	db "Hello world!", 13, 10, "$"

end 		Start