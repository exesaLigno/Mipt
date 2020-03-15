SECTION .TEXT
    GLOBAL forout

;Parameters sending throw RDI, RSI, RDX, RCX, R8, R9

;R15 - parameter_counter


forout:         push r9             ;|
                push r8             ;|
                push rcx            ;|
                push rdx            ;|
                push rsi            ;| Pushing parameters from end to start
                mov rsi, rdi

                call printer
                
                ;add rsi, rdx
                ;add rsi, 2
                
                pop rax
                pop rax
                pop rax
                pop rax
                pop rax
                
                mov rax, r14
                
                ret
                

;-----------------------------------------------;
; Counting length of string                     ;
; ENTRY:    RSI - string offset                 ;
; EXIT:     RDX - strlen(string)                ;
; DESTR:    RDX                                 ;
;-----------------------------------------------;
getstrlen:      xor rdx, rdx
                push rsi
    
    .cycle:     cmp byte [rsi], 0x0
                je .exit
                cmp byte [rsi], '%'
                je .exit
                
                inc rsi
                inc rdx
                
                jmp .cycle
                
    .exit:      pop rsi
                
                ret
                
                
                
;-------------------------------------------------------;
; Printing format string and calling parameters         ;
; parser when it necessary                              ;
; ENTRY:    RSI - format string                         ;
;           Stack - parameters                          ;
; EXIT:     R14 - count of printed symbols              ;
; DESTR:    RAX, RSI, RDI, RDX, R15, R14                ;
;-------------------------------------------------------;
printer:        mov r15, 24        ;Initail parameters indent
                xor r14, r14
                
    .cycle:     mov rax, 0x01
                mov rdi, 1
                call getstrlen
                
                syscall
                
                add r14, rdx
                
                add rsi, rdx
                
                cmp byte [rsi], '%'
                jne .continue
                add r15, 8
                call parser
                
    .continue:  cmp byte [rsi], 0x0
                
                jne .cycle
                
                ret
                
                
;----------------------------------------------;
; Parameters parser                            ;
; ENTRY:    RSI - format string                ;
; DESTR:    RSI                                ;
;----------------------------------------------;            
parser:         inc rsi

                cmp byte [rsi], 'b'
                jne .octal
                call binary
                
    .octal:     cmp byte [rsi], 'o'
                jne .decimal
                call octal
                
    .decimal:   cmp byte [rsi], 'd'
                jne .hex
                call decimal
                
    .hex:       cmp byte [rsi], 'h'
                jne .char
                call hex
                
    .char:      cmp byte [rsi], 'c'
                jne .string
                call char
                
    .string:    cmp byte [rsi], 's'
                jne .exit
                call string
    
    .exit:      inc rsi
                
                ret
                

;----------------------------------------------;
; Char printer                                 ;
; ENTRY:    R15 - Stack indent                 ;
;           Stack[R15] - symbol                ;
; DESTR:    RAX, RDI, RDX, R14                 ;
;----------------------------------------------;
char:           push rsi
                mov rsi, rsp
                add rsi, r15
                
                mov rax, 0x01
                mov rdi, 1
                mov rdx, 1
                syscall
                
                add r14, 1
                
                pop rsi                
                
                ret
                

;-----------------------------------------------;
; String printer                                ;
; ENTRY:    R15 - Stack indent                  ;
;           Stack[R15] - string adress          ;
; DESTR:    RAX, RDI, RDX, R14                  ;
;-----------------------------------------------;
string:         push rsi
                mov rsi, [rsp + r15]
                
                mov rax, 0x01
                mov rdi, 1
                call getstrlen
                
                syscall
                
                add r14, rdx

                pop rsi
            
                ret
                
                
binary:         push rsi
                mov rsi, [rsp + r15]
                xor r8, r8
                
    .cycle1:    mov r13, rsi
                and rsi, 01b
                push rsi
                inc r8
                mov rsi, r13
                shr rsi, 1
                cmp rsi, 0
                jne .cycle1
                
    .cycle2:    pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                mov rax, 0x01
                mov rdi, 1
                mov rdx, 1
                syscall
                
                dec r8
                
                cmp r8, 0
                jne .cycle2                
                
                pop rsi
                
                ret

octal:          push rsi
                mov rsi, [rsp + r15]
                xor r8, r8
                
    .cycle1:    mov r13, rsi
                and rsi, 0111b
                push rsi
                inc r8
                mov rsi, r13
                shr rsi, 3
                cmp rsi, 0
                jne .cycle1
                
    .cycle2:    pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                mov rax, 0x01
                mov rdi, 1
                mov rdx, 1
                syscall
                
                dec r8
                
                cmp r8, 0
                jne .cycle2                
                
                pop rsi
                
                ret

hex:            push rsi
                mov rsi, [rsp + r15]
                xor r8, r8
                
    .cycle1:    mov r13, rsi
                and rsi, 01111b
                push rsi
                inc r8
                mov rsi, r13
                shr rsi, 4
                cmp rsi, 0
                jne .cycle1
                
    .cycle2:    pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                mov rax, 0x01
                mov rdi, 1
                mov rdx, 1
                syscall
                
                dec r8
                
                cmp r8, 0
                jne .cycle2                
                
                pop rsi
                
                ret

decimal:        ret


NUMBERS:        db "0123456789abcdef"





