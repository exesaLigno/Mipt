# CPU emulator
## Using
To use emulator compile "assembler.cpp", "CPU.cpp" and "disasembler.cpp". After that start assembler with name of source code.
```
./assembler.o equation.txt
```
After that start start CPU emulator
```
./CPU.o equation.core
```
## Source
All commands for CPU containing in "commands.h". Instructions for the processor are written as DEF_CMD(*<name_of_command>*, *<command>*, *<number_of_command>*, *{command_instructions}*).
For now assembler contains the commands below:
```
push
pop
add
sub
mul
div
out
end
jmp
je
jne
ja
jae
jb
jbe
call
ret
flag
swap
in
sqrt
sqr
pow
```
Examples of source code contained in repo in .txt format



