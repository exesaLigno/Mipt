all: 
	nasm -f elf64 functions.s -o functions.o
	gcc -fPIE main.c functions.o -o program.out
	
	
.PHONY: clean
clean:
	rm -rf *.o program.out
	
.PHONY: run
run:
	./program.out
