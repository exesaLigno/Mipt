CC=g++
CFLAGS=-c -std=c++2a -O3
LDFLAGS=

JC_SOURCES=jc.cpp Source/compiler.cpp Source/source.cpp Source/ast.cpp Source/binary.cpp
JC_OBJECTS=$(JC_SOURCES:.cpp=.o)
JC=jc

JVM_SOURCES=jvm.cpp
JVM_OBJECTS=$(JVM_SOURCES:.cpp=.o)
JVM=jvm

ELFPARSER_SOURCES=Accessory/readBinary.cpp
ELFPARSER_OBJECTS=$(ELFPARSER_SOURCES:.cpp=.o)
ELFPARSER=relf


all: $(JC) $(JVM) $(ELFPARSER)
	
	
$(JC): $(JC_OBJECTS) 
	$(CC) $(LDFLAGS) $(JC_OBJECTS) -o $@
	
$(JVM): $(JVM_OBJECTS)
	$(CC) $(LDFLAGS) $(JVM_OBJECTS) -o $@
	
$(ELFPARSER): $(ELFPARSER_OBJECTS)
	$(CC) $(LDFLAGS) $(ELFPARSER_OBJECTS) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
	
.PHONY: examples
examples: $(COMPILER)
	

	
.PHONY: clean
clean:
	rm -rf *.o */*.o $(JC) $(JVM) $(ELFPARSER)
	

.PHONY: nasm
nasm:
	nasm -f elf64 test.s -o test.o
	ld -s -o a.out test.o
