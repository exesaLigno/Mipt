section .text
    global forout
    
    
%macro exit 1
    push %1
    mov rax, 0x3C
    pop rdi
    syscall
%endmacro

;---------------------------------;
;   printString string*, strlen   ;
;---------------------------------;
%macro printString 2
    push rax
    push rdi
    push rsi
    push rdx

    mov rax, 0x01
    mov rdi, 1
    mov rsi, %1
    mov rdx, %2
    syscall
    add r14, rdx
    
    xor %2, %2
    
    pop rdx
    pop rsi
    pop rdi
    pop rax
%endmacro

%define parameter(depth) [rsp + 8*r15 + depth]
;// r15 - parameters counter
;// r14 - symbols counter
;// rcx - buffer counter
;// rsi - format string

forout:         pop r12             ;| Saving return adress

                push r9             ;|
                push r8             ;|
                push rcx            ;|
                push rdx            ;|
                push rsi            ;| Pushing parameters from end to start
                mov rsi, rdi

                call printer
                
                pop rax
                pop rax
                pop rax
                pop rax
                pop rax
                
                mov rax, r14        ;| Returning count of printed symbols
                
                push r12            ;| Restoring return adress
                
                ret
                

               
;-------------------------------------------------------;
; Printing format string and calling parameters         ;
; parser when it necessary                              ;
; ENTRY:    RSI - format string                         ;
;           Stack - parameters                          ;
; EXIT:     R14 - count of printed symbols              ;
; DESTR:    RAX, RDI, RDX, R15                          ;
;-------------------------------------------------------;
printer:        xor r15, r15
                xor r14, r14
                xor rcx, rcx
                mov rdx, BUFFER
                
    .cycle:     cmp byte [rsi], '%'    ;|
                jne .continue          ;| if (*rsi == '%')
                                       ;|     parser(rsi);
                call parser            ;|     continue;
                jmp .cycle             ;|
                
    .continue:  cmp byte [rsi], 0x0    ;| else if (*rsi == '\0')
                je .exit               ;|     return counter

                cmp rcx, BUFFER_LENGTH
                jne .cont
                printString BUFFER, rcx
    
        .cont:  mov al, byte [rsi]
                mov [rdx + rcx], al    ;| else
                inc rcx                ;|     BUFFER[rcx] = *rsi
                inc rsi                ;|
                
                jmp .cycle
                
    .exit:      printString BUFFER, rcx
                
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
; DESTR:    RCX                                ;
;----------------------------------------------;
char:           cmp rcx, BUFFER_LENGTH
                jne .cont
                printString BUFFER, rcx
    
        .cont:  mov rax, parameter(24)  ;| 24 is a parameters depth - number of elements above the start of parameters
                mov [rdx + rcx], al
                inc rcx
                
                inc r15
                
                ret
                

;-----------------------------------------------;
; String printer                                ;
; ENTRY:    R15 - Stack indent                  ;
;           Stack[R15] - string adress          ;
; DESTR:    RAX, RDI, RDX, R14                  ;
;-----------------------------------------------;
string:         push rsi
                mov rsi, parameter(32)
                
    .cycle      cmp rcx, BUFFER_LENGTH
                jne .cont
                printString BUFFER, rcx
    
        .cont:  cmp byte [rsi], 0x0
                je .exit
                mov al, byte [rsi]
                mov [rdx + rcx], al
                inc rcx
                inc rsi
                
                jmp .cycle
                
    .exit:      pop rsi
                inc r15
                ret
                
                
               
;------------------------------------------------;
; Number printer (base multiple of two)          ;
; ENTRY:    R15 - Stack indent                   ;
;           Stack[R15] - number                  ;
;           RAX - base                           ;
; DESTR:    RAX, RDI, RDX, R8, R13, R14          ;
;------------------------------------------------;
twodegreebase:  push rsi
                mov rsi, parameter(32)
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
                	
    .prcycle:   cmp rcx, BUFFER_LENGTH
                jne .cont
                printString BUFFER, rcx
    
        .cont:  pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                mov bl, [rsi]
                
                mov [rdx + rcx], bl
                
                inc rcx
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
                mov r9, rcx
                
                xor r8, r8
                
                mov rcx, rax
                mov rax, parameter(32)
                
    .trcycle:   xor rdx, rdx
                div rcx
                push rdx
                inc r8
                cmp rax, 0
                jne .trcycle
                
                mov rdx, BUFFER
                mov rcx, r9
                
    .prcycle:   cmp rcx, BUFFER_LENGTH
                jne .cont
                printString BUFFER, rcx
    
        .cont:  pop rdi
                mov rsi, NUMBERS
                add rsi, rdi
                mov bl, [rsi]
                
                mov [rdx + rcx], bl
                
                inc rcx
                dec r8
                
                cmp r8, 0
                jne .prcycle
                
                inc r15
                pop rsi
                ret


NUMBERS:        db "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"


section .bss
BUFFER:         resb 1024
BUFFER_LENGTH   equ $ - BUFFER





