#include "token.hpp"

TOKEN("+", 	Token::ARITHM_OPERATOR, Token::PLUS, 	 	 "+", "\tpop rax\n\tpop rbx\n\tadd rax, rbx\n\tpush rax\n\n")
TOKEN("-", 	Token::ARITHM_OPERATOR, Token::MINUS, 	 	 "-", "\tpop rax\n\tpop rbx\n\tsub rax, rbx\n\tpush rax\n\n")
TOKEN("*", 	Token::ARITHM_OPERATOR, Token::MULTIPLY, 	 "*", "\tpop rax\n\tpop rbx\n\timul ebx\n\tpush rax\n\n")
//TOKEN("/", 	Token::ARITHM_OPERATOR, Token::DIVIDE, 	 	 "/", "pop rax\npop rbx\nidiv ebx\npush rax\nнассыл в штаны\n\n")
//TOKEN("^", 	Token::ARITHM_OPERATOR, Token::POWER, 	 	 "^", "  N|I power\n\n")
TOKEN("%", 	Token::ARITHM_OPERATOR, Token::MODULO, 	 	 "%", "\tpop rax\n\tpop rbx\n\timul rbx\n\tpush rdx\n\n")
TOKEN("//", Token::ARITHM_OPERATOR, Token::INT_DIVISION, "//", "\tpop rax\n\tpop rbx\n\timul rbx\n\tpush rax\n\n")

TOKEN("==", Token::CMP_OPERATOR, Token::EQUAL, 		"==", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 01000000b\n\tshr cl, 6\n\tpush rcx\n\n")
TOKEN("!=", Token::CMP_OPERATOR, Token::NOT_EQUAL, 	"!=", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 01000000b\n\txor cl, 01000000b\n\tshr cl, 6\n\tpush rcx\n\n")
TOKEN(">=", Token::CMP_OPERATOR, Token::MORE_EQ, 	"More or equal", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 10000000b\n\txor cl, 10000000b\n\tshr cl, 7\n\tpush rcx\n\n")
TOKEN("<=", Token::CMP_OPERATOR, Token::LESS_EQ, 	"Less or equal", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\txor rdx, rdx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tmov dl, ah\n\tand cl, 01000000b\n\tshr cl, 6\n\tand dl, 10000000b\n\tshr dl, 7\n\tor rcx, rdx\n\tpush rcx\n\n")
TOKEN(">", 	Token::CMP_OPERATOR, Token::MORE, 		"More", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\txor rdx, rdx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tmov dl, ah\n\tand cl, 01000000b\n\txor cl, 01000000b\n\tshr cl, 6\n\tand dl, 10000000b\n\txor dl, 10000000b\n\tshr dl, 7\n\tand rcx, rdx\n\tpush rcx\n\n")
TOKEN("<", 	Token::CMP_OPERATOR, Token::LESS, 		"Less", "\tpop rax\n\tpop rbx\n\txor rcx, rcx\n\tcmp rax, rbx\n\tlahf\n\tmov cl, ah\n\tand cl, 10000000b\n\tshr cl, 7\n\tpush rcx\n\n")
TOKEN("&&", Token::CMP_OPERATOR, Token::AND, 		"&&", "\tpop rax\n\tpop rbx\n\tand rax, rbx\n\tpush rax\n\n")
TOKEN("||", Token::CMP_OPERATOR, Token::OR, 		"||", "\tpop rax\n\tpop rbx\n\tor rax, rbx\n\tpush rax\n\n")

TOKEN("if", 	Token::CTRL_OPERATOR, Token::IF, 					"if", "\x1b[1;31mIncorrect \'if\' compilation!\n\n\x1b[0m")
TOKEN("else", 	Token::CTRL_OPERATOR, Token::ELSE, 					"else", "\x1b[1;31mIncorrect \'else\' compilation!\n\n\x1b[0m")
TOKEN("for", 	Token::CTRL_OPERATOR, Token::FOR, 					"for", "\x1b[1;31mIncorrect \'for\' compilation!\n\n\x1b[0m")
TOKEN("while", 	Token::CTRL_OPERATOR, Token::WHILE, 				"while", "\x1b[1;31mIncorrect \'while\' compilation!\n\n\x1b[0m")
TOKEN("return", Token::CTRL_OPERATOR, Token::RETURN, 				"return", "\tpop rax\n\n")
TOKEN("=", 		Token::CTRL_OPERATOR, Token::ASSIGNMENT, 			"=", "\tpop rbx\n\tpop rax\n\tmov [rbx], rax\n\n")
TOKEN("+=", 	Token::CTRL_OPERATOR, Token::PLUS_ASSIGNMENT, 		"+=", "\tpop rbx\n\tpop rax\n\tadd [rbx], rax\n\n")
TOKEN("-=", 	Token::CTRL_OPERATOR, Token::MINUS_ASSIGNMENT, 		"-=", "\tpop rbx\n\tpop rax\n\tsub [rbx], rax\n\n")
//TOKEN("*=", 	Token::CTRL_OPERATOR, Token::MULTIPLY_ASSIGNMENT, 	"*=", "imul [rbx], rax\n\n")
//TOKEN("/=", 	Token::CTRL_OPERATOR, Token::DIVIDE_ASSIGNMENT, 	"/=", "idiv [rbx], rax\n\n")
//TOKEN("^=", 	Token::CTRL_OPERATOR, Token::POWER_ASSIGNMENT, 		"^=", "power [rbx], rax\n\n")



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
