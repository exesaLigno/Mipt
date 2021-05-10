;! Write right documentation
;! Add text to frame
;! Add buttons to frame

.model tiny
.code

include header.asm

org 100h

Start:		mov dx, video_segment	;|
		mov es, dx		;| es = VIDEO_SEGM

		mov bh, 5	; left top corner row
		mov bl, 15	; left top corner column
		mov cx, 50 	; frame width
		mov bp, 10	; frame height
		call DRAW_FRAME
		.getch
		.exitDOS


include lib.asm

String:		"Evetyone safe in treehouse", 0

end		Start