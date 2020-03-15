SECTION .TEXT
    GLOBAL forout

%macro printString 0
    mov rax, 0x01
    mov rdi, 1
    call getstrlen
    syscall
    add r14, rdx
%endmacro

%macro printChar 0
    mov rax, 0x01
    mov rdi, 1
    mov rdx, 1
    syscall
    add r14, 1
%endmacro

%define parameter [rsp + r15]


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
                
    .cycle:     printString
                
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
                jne .quaternary
                mov rax, 2
                call twodegreebase
                
    .quaternary:cmp byte [rsi], 'q'
                jne .octal
                mov rax, 4
                call twodegreebase
                
    .octal:     cmp byte [rsi], 'o'
                jne .hex
                mov rax, 8
                call twodegreebase
                
    .hex:       cmp byte [rsi], 'h'
                jne .decimal
                mov rax, 16
                call twodegreebase
                
    .decimal:   cmp byte [rsi], 'd'
                jne .char
                mov rax, 10
                call anybase
                
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
                
                printChar
                
                pop rsi                
                
                ret
                

;-----------------------------------------------;
; String printer                                ;
; ENTRY:    R15 - Stack indent                  ;
;           Stack[R15] - string adress          ;
; DESTR:    RAX, RDI, RDX, R14                  ;
;-----------------------------------------------;
string:         push rsi

                mov rsi, parameter
                printString
                
                pop rsi
                ret
                
                
               
;------------------------------------------------;
; Number printer (base multiple of two)          ;
; ENTRY:    R15 - Stack indent                   ;
;           Stack[R15] - number                  ;
;           RAX - base                           ;
; DESTR:    RAX, RDI, RDX, R8, R13, R14          ;
;------------------------------------------------;
twodegreebase:  push rsi
                mov rsi, parameter
                xor r8, r8
                dec rax
                
    .trcycle:   mov r13, rsi
                and rsi, rax
                push rsi
                inc r8
                mov rsi, r13
                
                    push rax
        .shcycle:   shr rax, 1
                    shr rsi, 1
                    cmp rax, 0
                    jne .shcycle
                    pop rax
                
                cmp rsi, 0
                jne .trcycle
                
    .prcycle:   pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                
                printChar
                
                dec r8
                
                cmp r8, 0
                jne .prcycle             
                
                pop rsi
                
                ret


;------------------------------------------------;
; Number printer (any base)                      ;
; ENTRY:    R15 - Stack indent                   ;
;           Stack[R15] - number                  ;
;           RAX - base                           ;
; DESTR:    RAX, RDI, RDX, RCX, R8, R14          ;
;------------------------------------------------;
anybase:        push rsi
                
                xor r8, r8
                
                mov rcx, rax
                mov rax, parameter
                
    .trcycle:   xor rdx, rdx
                div rcx
                push rdx
                inc r8
                cmp rax, 0
                jne .trcycle
                
                
    .prcycle:   pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                
                printChar
                
                dec r8
                
                cmp r8, 0
                jne .prcycle
                
                pop rsi
                ret


NUMBERS:        db "0123456789abcdefghijklmnopqrstuvwxyz"





