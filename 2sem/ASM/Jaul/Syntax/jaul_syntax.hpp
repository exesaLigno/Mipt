/*!
 *	@file jaul_syntax.hpp
 *	@brief Инструкции для компиляции высокоуровневых операций в ассемблерный код
 */

#include "../Headers/ast.hpp"

TOKEN("+", 	ASN::ARITHM_OPERATOR, ASN::PLUS, 	 	 "+", "pop rax\npop rbx\nadd rax, rbx\npush rax", "58 5b 48 01 d8 50")
TOKEN("-", 	ASN::ARITHM_OPERATOR, ASN::MINUS, 	 	 "-", "pop rax\npop rbx\nsub rax, rbx\npush rax", "58 5b 48 29 d8 50")
TOKEN("*", 	ASN::ARITHM_OPERATOR, ASN::MULTIPLY, 	 "*", "pop rax\npop rbx\nimul ebx\npush rax", "58 5b f7 eb 50")
//TOKEN("/", 	ASN::ARITHM_OPERATOR, ASN::DIVIDE, 	 	 "/", "pop rax\npop rbx\nidiv ebx\npush rax\nнассыл в штаны")
//TOKEN("^", 	ASN::ARITHM_OPERATOR, ASN::POWER, 	 	 "^", "  N|I power")
TOKEN("%", 	ASN::ARITHM_OPERATOR, ASN::MODULO, 	 	 "%", "pop rax\npop rbx\nidiv rbx\npush rdx", "58 5b 48 f7 fb 52")
TOKEN("//", ASN::ARITHM_OPERATOR, ASN::INT_DIVISION, "//", "pop rax\npop rbx\nidiv rbx\npush rax", "58 5b 48 f7 fb 50")

TOKEN("==", ASN::CMP_OPERATOR, ASN::EQUAL, 		"==", "pop rax\npop rbx\nxor rcx, rcx\ncmp rax, rbx\nlahf\nmov cl, ah\nand cl, 01000000b\nshr cl, 6\npush rcx", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 40 c0 e9 06 51")
TOKEN("!=", ASN::CMP_OPERATOR, ASN::NOT_EQUAL, 	"!=", "pop rax\npop rbx\nxor rcx, rcx\ncmp rax, rbx\nlahf\nmov cl, ah\nand cl, 01000000b\nxor cl, 01000000b\nshr cl, 6\npush rcx", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 40 80 f1 40 c0 e9 06 51")
TOKEN(">=", ASN::CMP_OPERATOR, ASN::MORE_EQ, 	"More or equal", "pop rax\npop rbx\nxor rcx, rcx\ncmp rax, rbx\nlahf\nmov cl, ah\nand cl, 10000000b\nxor cl, 10000000b\nshr cl, 7\npush rcx", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 80 80 f1 80 c0 e9 07 51")
TOKEN("<=", ASN::CMP_OPERATOR, ASN::LESS_EQ, 	"Less or equal", "pop rax\npop rbx\nxor rcx, rcx\nxor rdx, rdx\ncmp rax, rbx\nlahf\nmov cl, ah\nmov dl, ah\nand cl, 01000000b\nshr cl, 6\nand dl, 10000000b\nshr dl, 7\nor rcx, rdx\npush rcx", "58 5b 48 31 c9 48 31 d2 48 39 d8 9f 88 e1 88 e2 80 e1 40 c0 e9 06 80 e2 80 c0 ea 07 48 09 d1 51")
TOKEN(">", 	ASN::CMP_OPERATOR, ASN::MORE, 		"More", "pop rax\npop rbx\nxor rcx, rcx\nxor rdx, rdx\ncmp rax, rbx\nlahf\nmov cl, ah\nmov dl, ah\nand cl, 01000000b\nxor cl, 01000000b\nshr cl, 6\nand dl, 10000000b\nxor dl, 10000000b\nshr dl, 7\nand rcx, rdx\npush rcx", "58 5b 48 31 c9 48 31 d2 48 39 d8 9f 88 e1 88 e2 80 e1 40 80 f1 40 c0 e9 06 80 e2 80 80 f2 80 c0 ea 07 48 21 d1 51")
TOKEN("<", 	ASN::CMP_OPERATOR, ASN::LESS, 		"Less", "pop rax\npop rbx\nxor rcx, rcx\ncmp rax, rbx\nlahf\nmov cl, ah\nand cl, 10000000b\nshr cl, 7\npush rcx", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 80 c0 e9 07 51")
TOKEN("&&", ASN::CMP_OPERATOR, ASN::AND, 		"&&", "pop rax\npop rbx\nand rax, rbx\npush rax", "58 5b 48 21 d8 50")
TOKEN("||", ASN::CMP_OPERATOR, ASN::OR, 		"||", "pop rax\npop rbx\nor rax, rbx\npush rax", "58 5b 48 09 d8 50")

TOKEN("if", 	ASN::CTRL_OPERATOR, ASN::IF, 					"if", "nop", "")
TOKEN("else", 	ASN::CTRL_OPERATOR, ASN::ELSE, 					"else", "nop", "")
TOKEN("for", 	ASN::CTRL_OPERATOR, ASN::FOR, 					"for", "nop", "")
TOKEN("while", 	ASN::CTRL_OPERATOR, ASN::WHILE, 				"while", "nop", "")
TOKEN("return", ASN::CTRL_OPERATOR, ASN::RETURN, 				"return", "pop rax", "58")
TOKEN("=", 		ASN::CTRL_OPERATOR, ASN::ASSIGNMENT, 			"=", "pop rbx\npop rax\nmov [rbx], rax", "5b 58 48 89 03")
//TOKEN("+=", 	ASN::CTRL_OPERATOR, ASN::PLUS_ASSIGNMENT, 		"+=", "pop rbx\npop rax\nadd [rbx], rax")
//TOKEN("-=", 	ASN::CTRL_OPERATOR, ASN::MINUS_ASSIGNMENT, 		"-=", "pop rbx\npop rax\nsub [rbx], rax")
//TOKEN("*=", 	ASN::CTRL_OPERATOR, ASN::MULTIPLY_ASSIGNMENT, 	"*=", "imul [rbx], rax")
//TOKEN("/=", 	ASN::CTRL_OPERATOR, ASN::DIVIDE_ASSIGNMENT, 	"/=", "idiv [rbx], rax")
//TOKEN("^=", 	ASN::CTRL_OPERATOR, ASN::POWER_ASSIGNMENT, 		"^=", "power [rbx], rax")



/*
%macro equal 0
	pop rax\npop rbx\nxor rcx, rcx\ncmp rax, rbx\nlahf\nmov cl, ah\nand cl, 01000000b\nshr cl, 6\npush rcx
%endmacro

%macro not_equal 0
	xor rcx, rcx
	cmp rax, rbx
	lahf
	mov cl, ah
	and cl, 01000000b
	xor cl, 01000000b
	shr cl, 6
	push rcx
%endmacro

%macro less 0
	xor rcx, rcx
	cmp rax, rbx
	lahf
	mov cl, ah
	and cl, 10000000b
	shr cl, 7
	push rcx
%endmacro

%macro less_eq 0
	xor rcx, rcx
	xor rdx, rdx
	cmp rax, rbx
	lahf
	mov cl, ah
	mov dl, ah
	and cl, 01000000b
	shr cl, 6
	and dl, 10000000b
	shr dl, 7
	or rcx, rdx
	push rcx
%endmacro

%macro more 0
	xor rcx, rcx
	xor rdx, rdx
	cmp rax, rbx
	lahf
	mov cl, ah
	mov dl, ah
	and cl, 01000000b
	xor cl, 01000000b
	shr cl, 6
	and dl, 10000000b
	xor dl, 10000000b
	shr dl, 7
	and rcx, rdx
	push rcx
%endmacro

%macro more_eq 0
	xor rcx, rcx
	cmp rax, rbx
	lahf
	mov cl, ah
	and cl, 10000000b
	xor cl, 10000000b
	shr cl, 7
	push rcx
%endmacro
*/
