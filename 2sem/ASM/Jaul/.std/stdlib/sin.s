sin:		fld dword [rsp + 8]
			fsin
			fst dword [rsp + 8]
			xor rax, rax
			mov eax, dword [rsp + 8]
			ret
