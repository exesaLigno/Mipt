SECTION .TEXT
    GLOBAL my_strlen
    GLOBAL my_strchr
    GLOBAL my_strrchr
    GLOBAL my_strcpy
                

;-----------------------------------------------;
; Counting length of string                     ;
; ENTRY:    RDI - string offset                 ;
; EXIT:     RAX - strlen(string)                ;
; DESTR:    RAX                                 ;
;-----------------------------------------------;
my_strlen:      xor rax, rax
    
    .cycle:     cmp byte [rdi], 0x0
                je .exit
                
                inc rdi
                inc rax
                
                jmp .cycle
                
    .exit:      ret
                
                
;-----------------------------------------------;
; Finding first entry of symbol in string       ;
; ENTRY:    RDI - string offset                 ;
;           RSI - symbol                        ;
; EXIT:     RAX - adress of first entry         ;
; DESTR:    RAX, RDX                            ;
;-----------------------------------------------;
my_strchr:      nop
                
                mov rdx, rsi
                
    .cycle:     cmp byte [rdi], dl
                je .exit
                
                inc rdi
                
                jmp .cycle
                
    .exit:      mov rax, rdi
                ret
                
                
                
;-----------------------------------------------;
; Finding first entry of symbol in string       ;
; ENTRY:    RDI - string offset                 ;
;           RSI - symbol                        ;
; EXIT:     RAX - adress of last entry          ;
; DESTR:    RAX, RDX                            ;
;-----------------------------------------------;
my_strrchr:     call my_strlen
                add rdi, rax
                
                mov rdx, rsi
                
    .cycle:     cmp byte [rdi], dl
                je .exit
                
                dec rdi
                
                jmp .cycle
                
    .exit:      mov rax, rdi
                ret
                
                
                
;-----------------------------------------------;
; Finding first entry of symbol in string       ;
; ENTRY:    RDI - string offset                 ;
;           RSI - symbol                        ;
; EXIT:     RAX - adress of last entry          ;
; DESTR:    RAX, RDX                            ;
;-----------------------------------------------;
my_strcpy:      call my_strlen
                add rdi, rax
                
                mov rdx, rsi
                
    .cycle:     cmp byte [rdi], dl
                je .exit
                
                dec rdi
                
                jmp .cycle
                
    .exit:      mov rax, rdi
                ret
