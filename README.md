# LLVM Count Pass
An LLVM pass that prints out the number of functions and basic blocks in a program along with generating a pdf containing a histogram of the number of instructions in each block.

## Dependencies
- Clang and LLVM 11.0
- gnuplot

## Setup
Copy the Count directory in to the folder SRC_ROOT/llvm/lib/Transforms and add the following line to SRC_ROOT/llvm/lib/Transforms/CMakeLists.txt:
```
add_subdirectory(Count)
```
where SRC_ROOT is the top level directory of the LLVM source tree. Run the `make` command from the top level directory to compile the new pass.

## Usage
To use the pass, first generate the LLVM bitcode version of the program by running
```
clang -c -emit-llvm program.c
```
The pass can then be used to count the number of functions and basic blocks by running
```
opt -load lib/LLVMCount.so -count < relative/path/to/program.bc > /dev/null
```
in the directory SRC_ROOT/build.

## Example
Running this pass on the following program:
```
#include <stdio.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("The program expects one argument\n");
    return 1;
  }
  
  if (argv[1][0] == 'A')
    printf("A\n");
  else 
    if (argv[1][0] == 'B')
      printf("B\n");
    else printf("C\n");
  
  return 0;
}
```
will produce the output:
```
The program has a total of 1 functions and 10 basic blocks.
1: 1
2: 5
3: 1
8: 2
9: 1
```
