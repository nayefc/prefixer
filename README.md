# prefixer

## Written by Nayef Copty.

Converts an infix expression into a prefix expression.
Build prefixer.c, run the binary file and supply an input file with an infix expression. Make sure to separate all numbers/characters with whitespaces.

Currently supports:
- Addition
- Subtraction
- Multiplication
- Division
- Variables and numbers as operands
- Postiive integers only

# Usage
## Prefixer:
### Build:
$ gcc prefixer.c
### Run: 
$ ./a.out file name
When running, please make sure that the file exists. You will get a parsing error if not instead of a "file not found" error.
I kept it this way to reduce the passing of variables. This would be needed for my test program below.

## Test:
### Build:
$ gcc test.c -o test
### Run:
$ ./test

In order to use my automated testing class, please make sure the prefixer.c binary file is called "./a.out"

Output format:
==============
All operators and operands will be separated by whitespace apart from the first operator/operand. Output will be followed by a whitespace and a newline (\n) character. The output is also wrapped in parenthesis.

Automated testing:
==================
An auotmated testing file is provided, test.c. The test file will fork the prefixer program as a child process, and use Linux syscalls to pipe the prefixer's output
into its own file to check for its outcome. Please read the run_child method comments to see how passing of arguments have to be formatted.