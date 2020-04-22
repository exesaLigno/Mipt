;---------------------------------------------------------------;
; Function drawing a box					;
; ENTRY: BH - upper left corner row (y)				;
;	 BL - upper left corner column (x)			;
;	 CX - frame width					;
;	 BP - frame height					;
; DESTR: AX, BX, DX, DI, BP, CX					;
;---------------------------------------------------------------;
DRAW_FRAME	proc
		
		dec bl

		call CELL_RECOUNT
		.setsym left_top_corner
		stosw

		inc bl

		.setsym horizontal_line
		push cx
		call DRAW_LINE
		pop cx

		add bl, cl
		.setsym right_top_corner
		stosw

		sub bl, cl
		dec bl
		
		inc bh
		
		
@@CYCLE:	;{
			call CELL_RECOUNT
			.setsym vertical_line
			stosw
	
			inc bl

			.setsym
			push cx
			call DRAW_LINE
			pop cx
	
			add bl, cl
			.setsym vertical_line
			stosw

			sub bl, cl
			dec bl
		
			inc bh
			dec bp

			jne @@CYCLE
		;}

		call CELL_RECOUNT
		.setsym left_bottom_corner
		stosw

		inc bl

		.setsym horizontal_line
		push cx
		call DRAW_LINE
		pop cx

		add bl, cl
		.setsym right_bottom_corner
		stosw

		sub bl, cl
		dec bl
		
		
		ret
		endp


;---------------------------------------------------------------;
; Function drawing a line					;
; ENTRY: BH - display row					;
;	 BL - display column					;
;	 AX - info about cell (AH - color, AL - symbol)		;
;	 CX - line length					;
; DESTR: DX, DI, CX						;
;---------------------------------------------------------------;
DRAW_LINE	proc
		
		push ax
		call CELL_RECOUNT
		pop ax

		rep stosw
		
		ret
		endp


;---------------------------------------------------------------;
; recounting di from row and column				;
; ENTRY: BH - display row					;
; 	 BL - display column					;
; DESTR: DX, AX							;
; RET:	 DI - cell number					;
;---------------------------------------------------------------;
CELL_RECOUNT	proc

		xor di, di
		
		mov dx, bx		;|
		and dx, 0FF00h		;| dx = row
		shr dx, 8		;|

		mov ax, 160d		;|
		dec dx			;| di = dx * 160
		mul dl			;|
		mov di, ax		;|

		mov dx, bx		;|
		and dx, 0FFh		;| dx = column

		shl dx, 1		;| 
		add di, dx		;| di += dx

		ret
		endp