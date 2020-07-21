CC=g++
CFLAGS=-c -std=c++2a -O3
LDFLAGS=

JC_SOURCES=jc.cpp Source/compiler.cpp Source/source.cpp Source/ast.cpp Source/binary.cpp Source/elf.cpp
JC_OBJECTS=$(JC_SOURCES:.cpp=.o)
JC=jc

JVM_SOURCES=jvm.cpp
JVM_OBJECTS=$(JVM_SOURCES:.cpp=.o)
JVM=jvm

ELFPARSER_SOURCES=Accessory/ReadBinary/readBinary.cpp
ELFPARSER_OBJECTS=$(ELFPARSER_SOURCES:.cpp=.o)
ELFPARSER=relf


all: $(JC) $(JVM) $(ELFPARSER) documentation
	
	
$(JC): $(JC_OBJECTS)
	$(CC) $(LDFLAGS) $(JC_OBJECTS) -o $@
	
$(JVM): $(JVM_OBJECTS)
	$(CC) $(LDFLAGS) $(JVM_OBJECTS) -o $@
	
$(ELFPARSER): $(ELFPARSER_OBJECTS)
	$(CC) $(LDFLAGS) $(ELFPARSER_OBJECTS) -o $@

.PHONY: documentation
documentation: Doxyfile
	doxygen Doxyfile


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
	
.PHONY: examples
examples: $(JC) $(JVM)


.PHONY: install
install: $(JC) $(JVM)
	cp $(JC) /usr/bin/$(JC)
	cp $(JVM) /usr/bin/$(JVM)
	rm -rf *.o */*.o $(JC) $(JVM) $(ELFPARSER)
	
	
.PHONY: uninstall
uninstall:
	rm -rf /usr/bin/$(JC) /usr/bin/$(JVM)

	
.PHONY: clean
clean:
	rm -rf *.o */*.o $(JC) $(JVM) $(ELFPARSER) *~ */*~
	

.PHONY: nasm
nasm: $(ELFPARSER)
	nasm -f elf64 nasm.s -o nasm.o
	ld -s -o compiled_nasm nasm.o
	rm nasm.o
	./$(ELFPARSER) compiled_nasm compiled_nasm.txt
	#rm compiled_nasm
	
