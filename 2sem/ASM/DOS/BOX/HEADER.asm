local @@
.getch		macro
		nop
		xor ah, ah
		int 16h
		nop
		endm

.exitDOS	macro ret
		nop
		ifnb <ret>
			mov ax, 4c00 or (ret and 0FFh)
		else
			mov ah, 4ch
		endif
		int 21h
		nop
		endm

.setsym		macro symbol
		nop
		ifnb <symbol>
			mov ax, (color shl 8) or symbol
		else
			mov ax, (color shl 8) or ' '
		endif
		nop
		endm

video_segment = 0b800h
color = 5Eh

vertical_line = 0BAh
horizontal_line = 0CDh
left_top_corner = 0C9h
right_top_corner = 0BBh
left_bottom_corner = 0C8h
right_bottom_corner = 0BCh
