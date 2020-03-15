all: 
	nasm -f elf64 printf.s -o printf.o
	gcc -fPIE main.c printf.o -o program.out
	
	
.PHONY: clean
clean:
	rm -rf *.o program.out
	
.PHONY: run
run:
	./program.out
