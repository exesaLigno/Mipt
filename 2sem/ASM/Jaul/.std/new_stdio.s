section .text
	global input
	global print
	

			
input:		sub rsp, 16			;// creating local buffer for reading
			mov r15, rsp
			mov r14, rsp
			
			mov rax, 0x00
			mov rdi, 0
			mov rsi, r15
			mov rdx, 16
			syscall				;// in RAX stores count of readed symbols
			
			add r14, rax		;// current position
			sub r14, 2
			
			mov rdi, 1			;// power of current symbol in RDX
			
			sub rsp, 8			;// creating two dword variables for mantissa and exhibitor
			mov dword [rsp], 1
			
			
	.cycle:	xor rax, rax
			mov al, byte [r14]
			
			cmp al, '.'
			je .point
			
			cmp al, ','
			je .point
			
			cmp al, '-'
			je .minus
			
			sub rax, '0'
			
			mul edi
			imul edi, 10
			add dword [rsp + 4], eax
			dec r14
			
			cmp r14, r15
			jge .cycle
			jmp .plus
			
			
	.point:	mov dword [rsp], edi
			dec r14
			jmp .cycle
			
			
	.minus:	fild dword [rsp + 4]
			fild dword [rsp]
			
			fdiv
			fchs
			
			fstp dword [rsp + 4]
			
			xor rax, rax
			mov eax, dword [rsp + 4]
			movd xmm0, dword [rsp + 4]
			
			jmp .exit
						
			
	.plus:	fild dword [rsp + 4]
			fild dword [rsp]
			
			fdiv
			
			fstp dword [rsp + 4]
			
			xor rax, rax
			mov eax, dword [rsp + 4]
			movd xmm0, dword [rsp + 4]
			
			
	.exit:	add rsp, 24
			
			ret
			
print:		sub rsp, 8
			movq qword [rsp], xmm0	;// C-decl, remake for jaul
			
			fld qword [rsp]
			
			mov rax, 1000000
			push rax
			fild qword [rsp]
			
			fmul
			frndint
			
			fistp qword [rsp]
			mov rax, qword [rsp]
			
			mov r15, rsp
			dec r15			;// last symbol pointer
			mov r14, 1		;// count of symbols
			mov r13, 6		;// symbols after point
			
			mov r11, 0
			cmp rax, 0
			jnl .skip
			mov r11, 1
			neg rax
			
	.skip:	sub rsp, 32
			
			mov byte [r15], 0xA
			dec r15
			
			mov r12, 0		;// skiping zeros
			
	.cycle:		xor rdx, rdx
				mov rbx, 10
				div rbx
				
				cmp r13, 0
				je .cont2
				
				cmp dl, 0
				je .cont1
				mov r12, 1
				
		.cont1:	cmp r12, 0
				jne .cont2
				
				dec r13
				jmp .cycle
				
		.cont2:	mov r12, 1
				add dl, '0'
				mov byte [r15], dl
				dec r15
				inc r14
				dec r13
				
				cmp r13, 0
				je .point
				
				cmp rax, 0
				jne .cycle
			
			jmp .sign
			
	.point:	mov byte [r15], '.'
			dec r15
			inc r14
			dec r13
			mov r12, 1
			jmp .cycle
			
	.sign:	cmp r11, 0
			je .sys
			mov byte [r15], '-'
			dec r15
			inc r14
			
	.sys:	inc r15
			mov rax, 0x01
			mov rdi, 1
			mov rsi, r15
			mov rdx, r14
			syscall
			
			add rsp, 32
			add rsp, 16
			
			
			
			ret
	
