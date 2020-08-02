sqr:		fld dword [rsp + 8]
			fmul st0, st0
			fst dword [rsp + 8]
			xor rax, rax
			mov eax, dword [rsp + 8]
			ret
