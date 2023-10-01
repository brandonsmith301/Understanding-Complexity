# Assembler Project
===

This project aims to convert assembly language into machine code. It consists of Python scripts that handle the tokenization, parsing, and code generation phases of an assembler. While not as efficient as other implementations when compared to C++ or other higher-performing languages, this was designed for intuitive understanding through Python.

## Dependencies

Before running any script, make sure to install the required Python packages. Run the following shell script to automatically install all dependencies:

```
./scripts.sh
```

## Directory 
```
|—— asm.py 
|———— Purpose: Main driver for assembler. 
|———— Optimisations: Avoids redundant disk reads by performing a two pass assembly.
|—— generator.py
|———— Purpose: Responsible for converting parsed instructions into machine code.
|———— Optimisations: Uses bitwise operations for generating machine code, improving the script's speed.
|—— lexer.py
|———— Purpose: Tokenizes the assembly file, converting it into a stream of tokens for easier parsing.
|———— Optimisations: Employs the FuzzyWuzzy library for improved error tolerance. Also, it tokenizes the file in a single pass, reducing I/O operations.
|—— nandparser.py
|———— Purpose: Handles the parsing logic, breaking down each line of assembly code into its constituent fields.
|———— Optimisations: Utilises regular expressions for efficient string manipulation.
|—— symboltable.py
|———— Purpose: Manages the symbol table to store variable names and labels.
|———— Optimisations: N/A ~ look ups done with dictionaries.

```
## How to Use
1. Install dependencies by running `./scripts.sh`
2. Run the assembler using the command:
```
python3 asm.py [filename.asm]
```
3. The machine code will be generated in a file with the same name as the input but with a `.hack` extension.