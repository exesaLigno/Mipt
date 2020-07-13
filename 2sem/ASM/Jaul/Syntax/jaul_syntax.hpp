#include "../Headers/ast.hpp"

TOKEN("+", 	ASN::ARITHM_OPERATOR, ASN::PLUS, 	 	 "+", "\tpop rax\n\tpop rbx\n\tadd rax, rbx\n\tpush rax\n\n", "58 5b 48 01 d8 50")
TOKEN("-", 	ASN::ARITHM_OPERATOR, ASN::MINUS, 	 	 "-", "\tpop rax\n\tpop rbx\n\tsub rax, rbx\n\tpush rax\n\n", "58 5b 48 29 d8 50")
TOKEN("*", 	ASN::ARITHM_OPERATOR, ASN::MULTIPLY, 	 "*", "\tpop rax\n\tpop rbx\n\timul ebx\n\tpush rax\n\n", "58 5b f7 eb 50")
//TOKEN("/", 	ASN::ARITHM_OPERATOR, ASN::DIVIDE, 	 	 "/", "pop rax\npop rbx\nidiv ebx\npush rax\nнассыл в штаны\n\n")
//TOKEN("^", 	ASN::ARITHM_OPERATOR, ASN::POWER, 	 	 "^", "  N|I power\n\n")
TOKEN("%", 	ASN::ARITHM_OPERATOR, ASN::MODULO, 	 	 "%", "\tpop rax\n\tpop rbx\n\tidiv rbx\n\tpush rdx\n\n", "58 5b 48 f7 fb 52")
TOKEN("//", ASN::ARITHM_OPERATOR, ASN::INT_DIVISION, "//", "\tpop rax\n\tpop rbx\n\tidiv rbx\n\tpush rax\n\n", "58 5b 48 f7 fb 50")

TOKEN("==", ASN::CMP_OPERATOR, ASN::EQUAL, 		"==", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 01000000b\n\tshr cl, 6\n\tpush rcx\n\n", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 40 c0 e9 06 51")
TOKEN("!=", ASN::CMP_OPERATOR, ASN::NOT_EQUAL, 	"!=", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 01000000b\n\txor cl, 01000000b\n\tshr cl, 6\n\tpush rcx\n\n", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 40 80 f1 40 c0 e9 06 51")
TOKEN(">=", ASN::CMP_OPERATOR, ASN::MORE_EQ, 	"More or equal", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 10000000b\n\txor cl, 10000000b\n\tshr cl, 7\n\tpush rcx\n\n", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 80 80 f1 80 c0 e9 07 51")
TOKEN("<=", ASN::CMP_OPERATOR, ASN::LESS_EQ, 	"Less or equal", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\txor rdx, rdx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tmov dl, ah\n\tand cl, 01000000b\n\tshr cl, 6\n\tand dl, 10000000b\n\tshr dl, 7\n\tor rcx, rdx\n\tpush rcx\n\n", "58 5b 48 31 c9 48 31 d2 48 39 d8 9f 88 e1 88 e2 80 e1 40 c0 e9 06 80 e2 80 c0 ea 07 48 09 d1 51")
TOKEN(">", 	ASN::CMP_OPERATOR, ASN::MORE, 		"More", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\txor rdx, rdx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tmov dl, ah\n\tand cl, 01000000b\n\txor cl, 01000000b\n\tshr cl, 6\n\tand dl, 10000000b\n\txor dl, 10000000b\n\tshr dl, 7\n\tand rcx, rdx\n\tpush rcx\n\n", "58 5b 48 31 c9 48 31 d2 48 39 d8 9f 88 e1 88 e2 80 e1 40 80 f1 40 c0 e9 06 80 e2 80 80 f2 80 c0 ea 07 48 21 d1 51")
TOKEN("<", 	ASN::CMP_OPERATOR, ASN::LESS, 		"Less", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 10000000b\n\tshr cl, 7\n\tpush rcx\n\n", "58 5b 48 31 c9 48 39 d8 9f 88 e1 80 e1 80 c0 e9 07 51")
TOKEN("&&", ASN::CMP_OPERATOR, ASN::AND, 		"&&", "\tpop rax\n\tpop rbx\n\tand rax, rbx\n\tpush rax\n\n", "58 5b 48 21 d8 50")
TOKEN("||", ASN::CMP_OPERATOR, ASN::OR, 		"||", "\tpop rax\n\tpop rbx\n\tor rax, rbx\n\tpush rax\n\n", "58 5b 48 09 d8 50")

TOKEN("if", 	ASN::CTRL_OPERATOR, ASN::IF, 					"if", "\tnop\n", "")
TOKEN("else", 	ASN::CTRL_OPERATOR, ASN::ELSE, 					"else", "\tnop\n", "")
TOKEN("for", 	ASN::CTRL_OPERATOR, ASN::FOR, 					"for", "\tnop\n", "")
TOKEN("while", 	ASN::CTRL_OPERATOR, ASN::WHILE, 				"while", "\tnop\n", "")
TOKEN("return", ASN::CTRL_OPERATOR, ASN::RETURN, 				"return", "\tpop rax\n\n", "58")
TOKEN("=", 		ASN::CTRL_OPERATOR, ASN::ASSIGNMENT, 			"=", "\tpop rbx\n\tpop rax\n\tmov [rbx], rax\n\n", "5b 58 48 89 03")
//TOKEN("+=", 	ASN::CTRL_OPERATOR, ASN::PLUS_ASSIGNMENT, 		"+=", "\tpop rbx\n\tpop rax\n\tadd [rbx], rax\n\n")
//TOKEN("-=", 	ASN::CTRL_OPERATOR, ASN::MINUS_ASSIGNMENT, 		"-=", "\tpop rbx\n\tpop rax\n\tsub [rbx], rax\n\n")
//TOKEN("*=", 	ASN::CTRL_OPERATOR, ASN::MULTIPLY_ASSIGNMENT, 	"*=", "imul [rbx], rax\n\n")
//TOKEN("/=", 	ASN::CTRL_OPERATOR, ASN::DIVIDE_ASSIGNMENT, 	"/=", "idiv [rbx], rax\n\n")
//TOKEN("^=", 	ASN::CTRL_OPERATOR, ASN::POWER_ASSIGNMENT, 		"^=", "power [rbx], rax\n\n")



/*
%macro equal 0
	\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 01000000b\n\tshr cl, 6\n\tpush rcx
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
