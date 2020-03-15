all: 
	nasm -f elf64 forout.s -o forout.o
	gcc -fPIE main.c forout.o -o program.out
	
	
.PHONY: clean
clean:
	rm -rf *.o program.out
	
.PHONY: run
run:
	./program.out
