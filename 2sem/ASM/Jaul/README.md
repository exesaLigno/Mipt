# JAUL Compiler Pack

JAUL means **J**ust **A**nother **U**seless **L**anguage.

## Contents

[Using - Compiler launch](#jc_usage)  
[Using - Running JVM](#jvm_usage)
[Compilation principle](#principle)  
[Object files](#object)  
[JAUL Virtual Mashine](#jvm)  
[Assembly](#assembly)  
[Examples](#examples)  


<a name="jc_usage"><h2>Using - Compiler launch</h2></a>

Compiler supports compilation keys:
`-v` or `--verbose` - verbose compilation (by default compiler shows only errors/warnings)

`-p` or `--only_preprocess` - only preprocess and save preprocessed code to output path

`-l` or `--nasm_listing` - make NASM listing it to output path

`--virtual` - compile code for [JVM](#jvm)

`--obj` - compile code and save it to [object file](#object)

`-h` or `--help` - show help and exit

`-o0`, `-o1`, `-o2` - optimization levels

`-o <filename>` - set output file name to `<filename>`.

If keys `-p`, `-l`, `--virtual` or `--obj` not given, compiling directly into ELF-64 executable.

Compiler takes three file types as input:
* .j - simple hi-level source ([Examples](#examples))
* .jo - [object files](#object)
* .jasm - special JAUL assembler language (Example of .jasm files at .std folder)


<a name="jvm_usage"><h2>Using - JVM</h2></a>

Using of [JVM](#jvm)


<a name="principle"><h2>Compilation principle</h2></a>

All source files importing to `Source` objects.

### JAUL Source
First of all, `Source` class building Abstract Syntax Tree and by which compiling instructions to `Binary` class object. 
After that `Binary` object optimizing and calculating local labels, byte code unites function by function 
(one function in one `Binary::Token`)

### JAUL Object
`Source` class just keeping byte code of object file, after that transmits it to `Binary` object. 
It builds object file function by function (one function in one `Binary::Token`).

### JAUL Assebler Language
It works like object files, but building `Binary` object basing on JASM code 
and after that optimizing and calculating local labels like in JAUL source compilation.

When all `Binary` objects are compiled, optimized and calculated, compiler linking them to one `Binary` object with all functions.
If object generation choosen, this `Binary` object just saving in file ([Object files](#object))
Else compiler calculating function labels in linked `Binary`, generating ELF-64 (or JVM) header and
saving all compiled code to executable file.


<a name="object"><h2>Object files</h2></a>

Object files format

<a name="jvm"><h2>JAUL Virtual Mashine</h2></a>

JAUL Virtual Mashine

<a name="assembly"><h2>Assembly</h2></a>

To assembly programm use `make` or `make jc` in repo directory. 
`make` building all programms in repository inlcuding readBinary ([see more](/Accessory/README.md)).


<a name="examples"><h2>Examples</h2></a>

Я сосал меня ебали



