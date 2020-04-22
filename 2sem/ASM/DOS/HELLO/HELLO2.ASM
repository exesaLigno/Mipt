
.model tiny
.code
org 100h

Start: 		mov ax, offset Message
		mov dx, offset EndOfMessage
		sub dx, ax			
		mov bl, dl			;bl = strlen(Message)

		mov si, offset Message		;dx = &Message		

		mov ah, 0eh
		mov cx, 01h
		mov bh, 00h			;setting up interrupt
	
Print:		mov al, [si]
		inc si
		dec bl
		int 10h
		cmp bl, 00h
		jne Print 
		
		mov ax, 4c00h
		int 21h


Message:	db "Somebody wants told me"
EndOfMessage:

end		Start