sin:		fild dword [rsp + 8]
			fstp dword [rsp + 8]
			xor rax, rax
			mov eax, dword [rsp + 8]
			ret
