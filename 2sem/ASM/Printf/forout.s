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
    
    pop rdx
    pop rsi
    pop rdi
    pop rax
%endmacro

%define parameter [rsp + r15]
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
                
                ;add rsi, rdx
                ;add rsi, 2
                
                pop rax
                pop rax
                pop rax
                pop rax
                pop rax
                
                mov rax, r14        ;| Returning count of printed symbols
                
                push r12            ;| Restoring return adress
                
                ret
                

;-----------------------------------------------;
; Counting length of string                     ;
; ENTRY:    RSI - string offset                 ; Надо ли?
; EXIT:     RDX - strlen(string)                ;
; DESTR:    RDX                                 ;
;-----------------------------------------------;
;getstrlen:      xor rdx, rdx
;                push rsi
;    
;    .cycle:     cmp byte [rsi], 0x0
;                je .exit
;                cmp byte [rsi], '%'
;                je .exit
;                
;                inc rsi
;                inc rdx
;                
;                jmp .cycle
;                
;    .exit:      pop rsi
;                
;                ret
;                
;                
;                
;-------------------------------------------------------;
; Printing format string and calling parameters         ;
; parser when it necessary                              ;
; ENTRY:    RSI - format string                         ;
;           Stack - parameters                          ;
; EXIT:     R14 - count of printed symbols              ;
; DESTR:    RAX, RDI, RDX, R15                          ;
;-------------------------------------------------------;
printer:        xor r15, r15;32        ;Initail parameters indent
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
    			
    			mov al, byte [rsi]
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

;                cmp byte [rsi], 'b'
;                jne .quaternary
;                mov rax, 2
;                call twodegreebase
;                
;    .quaternary:cmp byte [rsi], 'q'
;                jne .octal
;                mov rax, 4
;                call twodegreebase
;                
;    .octal:     cmp byte [rsi], 'o'
;                jne .hex
;                mov rax, 8
;                call twodegreebase
;                
;    .hex:       cmp byte [rsi], 'h'
;                jne .decimal
;                mov rax, 16
;                call twodegreebase
;                
;    .decimal:   cmp byte [rsi], 'd'
;                jne .char
;                mov rax, 10
;                call anybase
                
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
char:           mov rax, [rsp + 8 * r15 + 24]  ;| rsp (stack head) + r15 (param number) * 8 (sizeof(byte)) + 24 (parameters depth)
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
                mov rsi, [rsp + 8 * r15 + 32]
                
    .cycle      cmp byte [rsi], 0x0
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
;twodegreebase:  push rsi
;                mov rsi, parameter
;                xor r8, r8
;                dec rax
;                
;    .trcycle:   mov r13, rsi
;                and rsi, rax
;                push rsi
;                inc r8
;                mov rsi, r13
;                
;                    push rax
;        .shcycle:   shr rax, 1
;                    shr rsi, 1
;                    cmp rax, 0
;                    jne .shcycle
;                    pop rax
;                
;                cmp rsi, 0
;                jne .trcycle
;                
;    .prcycle:   pop rdi
;                mov rsi, NUMBERS
;                add rsi, rdi
;                
;                printChar
;                
;                dec r8
;                
;                cmp r8, 0
;                jne .prcycle             
;                
;                pop rsi
;                
;                ret


;------------------------------------------------;
; Number printer (any base)                      ;
; ENTRY:    R15 - Stack indent                   ;
;           Stack[R15] - number                  ;
;           RAX - base                           ;
; DESTR:    RAX, RDI, RDX, RCX, R8, R14          ;
;------------------------------------------------;
;anybase:        push rsi
;                
;                xor r8, r8
;                
;                mov rcx, rax
;                mov rax, parameter
;                
;    .trcycle:   xor rdx, rdx
;                div rcx
;                push rdx
;                inc r8
;                cmp rax, 0
;                jne .trcycle
;                
;                
;    .prcycle:   pop rdi
;                mov rsi, NUMBERS
;                add rsi, rdi
;                
;                printChar
;                
;                dec r8
;                
;                cmp r8, 0
;                jne .prcycle
;                
;                pop rsi
;                ret


NUMBERS:        db "0123456789abcdefghijklmnopqrstuvwxyz"

section .bss
BUFFER:			resb 1024
BUFFER_LENGTH   equ $ - BUFFER





