sqrt:		fld dword [rsp + 8]
			fsqrt
			fstp dword [rsp + 8]
			xor rax, rax
			mov eax, dword [rsp + 8]
			ret
