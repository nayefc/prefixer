# prefixer

## Written by Nayef Copty.

Converts an infix expression into a prefix expression.
Build prefixer.c, run the binary file and supply an input file with an infix expression.
Make sure to separate all numbers/characters with whitespaces.

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
When running, please make sure that the file exists. Otherwise, a parsing error will be thrown. The program does not check whether the infix notation is valid or not. 
I might introduce this in a later version. The program does not check that the expression is a valid infix expression. Please make sure that you have provided a valid expression.
Furthermore, the program will not work if you wrap the whole expresion between parenthesis.

### Test:
    $ ./test
    
    To add your own test cases, add them to `test.c`. Follow the conventions provided in the commented code. Once you're done, `make` and run `./test`.
    
### Clean
    $ make clean

# Output format:
All operators and operands will be separated by whitespace apart from the first operator/operand. Output will be followed by a whitespace and a newline (\n) character. The output is also wrapped in parenthesis.

# Automated testing:
An auotmated testing file is provided, test.c. The test file will fork the prefixer program as a child process, and use Linux syscalls to pipe the prefixer's output
into its own file to check for its outcome. Please read the run_child method comments to see how passing of arguments have to be formatted.

# Considerations:

### C vs. C++
I was contempalting whether to start in C or C++. Of course, C++ provides a lot of conveniences over C. I decided to go with C to make it a bit more challenging.

### Design Decisions
I started off the project by using a stack-based algorithm. Once I finished it, it was a little buggy in some caes. I also did not account for reducing the expressions.
I realized that using expression trees (which also utlizes a stack) will make it easier. I re-wrote the algorithm again to be based off an expression tree.
My stack is based off GNU's List implementation. This is inspired by C++'s list<> template. My stack does not require dynamic memory allocation and can take any `struct`, regarldess of typing, which has a pointer to a `struct_elem`.

All elements of an expression are of type `struct node`. A `node` contains a `union` which contains a `char` and an `int`. The point being is that each node will either hold an `int` or a `char`.

#### Reduction Algorithm
I remembered that I had to also reduce the expression further down the road in the project. I simplified the problem and pinpointed two different places where I'll be able to reduce an expression.
Either reduce the expression tree at the end, or take into account reduction as I build the tree. I had already created a `build_expression` function which keeps building all the sub-expressions into the stack, and finally, put the tree together.
To save time, and for the sake of efficiency, I decided to add all my reduction algorithm in there. Had I considered reduction in the beginning, I would have probably chose C++ as reducing the nodes recursively in C++ would be a lot easier.
I believe my reduction code could be simplified a lot. I could probably recursively reduce each level of the tree. I haven't put a lot of thought into that, neither have I studied how complex it will be in C, but if I had time to go back to this project, 
I will probably start by simplying my reduction code.


### Time Complexity
Given `n`, the length of the expression string (including whitespaces):

The outer while loop executes n times, since it iterates across every character in the expressions string. This has a complexity of O(n). 
The loop also calls `build_expression`. Building the expression tree is pretty much popping two elements from the operand stack, and popping one element from the operator stack. All of those operations have time complexity of O(1). So far, we have O(n) + O(1) = O(n).
At the end, a preorder traversal of the tree is going to take O(n), since each node will be visited once. The final time complexity for that is O(n).

