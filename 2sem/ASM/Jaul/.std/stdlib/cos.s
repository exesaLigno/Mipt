cos:		fld dword [rsp + 8]
			fcos
			fst dword [rsp + 8]
			xor rax, rax
			mov eax, dword [rsp + 8]
			ret
