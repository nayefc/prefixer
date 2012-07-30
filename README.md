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
## Make
    $ make
This will produce two executable files; `prefixer` and `test`.
## Run: 
### Prefixer:
    $ prefixer <file name> [-r]
When running, please make sure that the file exists. Otherwise, a parsing error will be thrown. The program does not check whether the infix notation is valid or not. I might introduce this in a later version.
The program does not check that the expression is a valid infix expression. Please make sure that you have provided a valid expression. Furthermore, the program will not work if you wrap the whole expresion \
between parenthesis.

### Test:
    $ ./test

Output format:
==============
All operators and operands will be separated by whitespace apart from the first operator/operand. Output will be followed by a whitespace and a newline (\n) character. The output is also wrapped in parenthesis.

Automated testing:
==================
An auotmated testing file is provided, test.c. The test file will fork the prefixer program as a child process, and use Linux syscalls to pipe the prefixer's output
into its own file to check for its outcome. Please read the run_child method comments to see how passing of arguments have to be formatted.

Considerations:
================
- union.... separation ints and chars to avoid a conflict between ex. 'a' and '97'
- reduction algorithm....
- C vs. C++....
