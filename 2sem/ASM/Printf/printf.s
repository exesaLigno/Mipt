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
                
    allexit:    mov rax, r14
                
                ret
                

;-----------------------------------------------;
; Counting length of string                     ;
; ENTRY:    RSI - string offset                 ;
; EXIT:     RDX - strlen(string)                ;
; DESTR:                                        ;
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
; EXIT:     RAX - count of printed symbols              ;
; DESTR:    Да ничего вроде    пока                     ;
; USING:    RAX, RDI, RSI, RDX, R15                     ;
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
                

binary:         ret

octal:          ret

decimal:        ret

hex:            ret

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
                

string:         push rsi
                mov rsi, [rsp + r15]
                
                mov rax, 0x01
                mov rdi, 1
                call getstrlen
                
                syscall
                
                add r14, rdx

                pop rsi
            
                ret


NUMBERS:        db "0123456789abcdef"





