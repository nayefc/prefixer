/*
 * prefixer.c
 * Created by: Nayef Copty
 */

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define OPEN_BRACKET '('
#define CLOSED_BRACKET ')'
#define ADDITION '+'
#define SUBTRACTION '-'
#define MULTIPLICATION '*'
#define DIVISION '/'

struct node {
    struct node *left;
    struct node *right;
    int isRightLeaf;
    int isTree;
    int isOperand;
    int isNumber;
    union {
	int number;
	char character;
    } value;
    struct stack_elem elem;
};

int isOperator(char p);
void build_expression(struct stack *operator_stack, struct stack *operand_stack, int reduce);
int precedence_calculator(char operator1, char operator2);
void reduce(struct node *root);
void constant_folding(struct node *root);
void preorder_traversal(struct node *root);
void free_tree(struct node *root);
void test_stack();

int main(int argc, char *argv[]) {

    test_stack();
    
    struct stack operator_stack;
    stack_init(&operator_stack);

    struct stack operand_stack;
    stack_init(&operand_stack);

    if (argc == 1) {
	fprintf(stderr, "Please pass in a file with an expression\n");
	exit(1);
    }

    int reduce_flag = 0;
    if (argc == 3) {
	if (strcmp(argv[2], "-r") == 0)
	    reduce_flag = 1;
    }
    
    FILE *fp;
    fp  = fopen(argv[1], "a+");
    rewind(fp);
    
    if (fp == NULL) {
	fprintf(stderr, "Unable to open file '%s'\n", argv[1]);
	exit(1);
    }

    char buf[50];    
    if (fgets(buf, 50, fp) == NULL) {
	fprintf(stderr, "Error in file parsing. Error: %s\n", strerror(errno));
	exit(1);
    }
    
    fclose(fp);
    char *p;
    p = buf;

    while (*p != '\0' && *p != '\n') {
	
	if (*p == ' ') {
	    p++;
	    continue;
	}
	
	if (isOperator(*p)) {
	    
	    struct node *operator_top;
	    if (!isEmpty(&operator_stack)) {
		struct stack_elem *e = peek(&operator_stack);
		operator_top = stack_entry(e, struct node, elem);
	    }

	    // if scanned character is open bracket, or operator stack is empty, or scanned character has higher precendence than stack top
	    if (*p == OPEN_BRACKET || isEmpty(&operator_stack) || precedence_calculator(*p, operator_top->value.character)) {

		struct node *node = (struct node *)malloc(sizeof(struct node));
		if (node == NULL) {
		    fprintf(stderr, "Out of memory\n");
		    exit(1);
		}
		node->value.character = *p;
		node->isOperand = 0;
		node->isNumber = -1;
		node->isTree = 0;
		push(&operator_stack, &node->elem);

		p++;
		continue;
	    }

	    else if (*p == CLOSED_BRACKET) {
		
		operator_top = stack_entry(peek(&operator_stack), struct node, elem);
		
		while (operator_top->value.character != '(') {
		    build_expression(&operator_stack, &operand_stack, reduce_flag);
		    operator_top = stack_entry(peek(&operator_stack), struct node, elem);
		}


		struct stack_elem *open_bracket_e = pop(&operator_stack);
		struct node *open_bracket = stack_entry(open_bracket_e, struct node, elem);
		free(open_bracket);

		p++;
		continue;
	    }

	    // if stack top has higher precendence than scanned character
	    else if (precedence_calculator(operator_top->value.character, *p)) {
				
		while (operator_top != NULL && precedence_calculator(operator_top->value.character, *p)) {
		    build_expression(&operator_stack, &operand_stack, reduce_flag);
		    if (isEmpty(&operator_stack))
			break;
		    struct stack_elem *e = peek(&operator_stack);
		    //		    e = e->next;
		    operator_top = stack_entry(e, struct node, elem);
		}
		
		struct node *node = (struct node *)malloc(sizeof(struct node));
		if (node == NULL) {
		    fprintf(stderr, "Out of memory\n");
		    exit(1);
		}
		
		node->value.character = *p;
		node->isOperand = 0;
		node->isNumber = -1;
		node->isTree = 0;
		push(&operator_stack, &node->elem);
		
		p++;
		continue;
	    }
	}

	// else if operand
	else {
	    
	    struct node *node = (struct node *)malloc(sizeof(struct node));
	    if (node == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	    }

	    if (isdigit(*p) || *p == '0') {
	
		int x = 0;
		sscanf(p, "%d", &x);
		node->value.number = x;
		node->isNumber = 1;
		push(&operand_stack, &node->elem);

		int digit_length;
		if (*p == '0')
		    digit_length = 1;
		else
		    digit_length = log10(x) + 1;

		int i;
		for (i = 0; i < digit_length; i++)
		    p++;
	    }

	    else {
		node->value.character = *p;
		node->isNumber = 0;
		push(&operand_stack, &node->elem);
		p++;
	    }

	    node->isOperand = 1;
	    node->isTree = 0;
	    node->left = NULL;
	    node->right = NULL;

	    continue;
	}
    }
    
    while (!isEmpty(&operator_stack))
	build_expression(&operator_stack, &operand_stack, reduce_flag);
    
    if (count(&operand_stack) > 1 && count(&operator_stack) > 0) {
	fprintf(stderr, "Error in algorithm.\n");
	exit(1);
    };
    
    struct stack_elem *root_element = peek(&operand_stack);
    struct node *root = stack_entry(root_element, struct node, elem);

    //    if (reduce_flag == 1)
    //	reduce(root);
    
    preorder_traversal(root);
    printf("\n");

    free_tree(root);
    
    return 0;
}

int isOperator(char p) {
    switch (p) {
    case '(':
	return 1;
	break;
    case ')':
	return 1;
	break;
    case '*':
	return 1;
	break;
    case '/':
	return 1;
	break;
    case '+':
	return 1;
	break;
    case '-':
	return 1;
	break;
    default:
	return 0;
    }
}

void build_expression(struct stack *operator_stack, struct stack *operand_stack, int reduce) {

    struct stack_elem *operator_element = pop(operator_stack);
    struct node *operator = stack_entry(operator_element, struct node, elem);
    operator->isRightLeaf = 0;
    
    struct stack_elem *operand2_element = pop(operand_stack);
    struct node *operand2 = stack_entry(operand2_element, struct node, elem);
    operand2->isRightLeaf = 1;
    
    struct stack_elem *operand1_element = pop(operand_stack);
    struct node *operand1 = stack_entry(operand1_element, struct node, elem);
    operand1->isRightLeaf = 0;

    if (reduce == 1) {

	// check that both operands are actually end operands and not operator nodes
	if (operand1->isOperand && operand2->isOperand) {
	    
	    // digit operand reductions
	    if (operand1->isNumber && operand2->isNumber) {

		int total = 0;

		if (operator->value.character == '*')
		    total = operand1->value.number * operand2->value.number;
		else if (operator->value.character == '/') {
		    if (operand2->value.number == 0) {
			fprintf(stderr, "Division by zero error.\n");
			exit(1);
		    }
		    total = operand1->value.number / operand2->value.number;
		}
		else if (operator->value.character == '+')
		    total = operand1->value.number + operand2->value.number;
		else if (operator->value.character == '-')
		    total = operand1->value.number - operand2->value.number;

		operator->isTree = 0;
		operator->value.number = total;
		operator->isNumber = 1;
		operator->isOperand = 1;
		operator->left = NULL;
		operator->right = NULL;
		push(operand_stack, &operator->elem);
	    }
	    
	    // non-digit operand reductions
	    else {
		
		if (operator->value.character == '*') {

		    if (operand1->isNumber && operand1->value.number == 1 ||
			operand2->isNumber && operand2->value.number == 1) {

			if (operand1->isNumber )
			    operator->value.character = operand2->value.character;
			else if (operand2->isNumber)
			    operator->value.character = operand1->value.character;
			
			operator->isNumber = 0;
			operator->isTree = 0;
			operator->isOperand = 1;
			operator->left = NULL;
			operator->right = NULL;
			push(operand_stack, &operator->elem);
		    }
		    
		    else if (operand1->isNumber && operand1->value.number == 0 ||
			operand2->isNumber && operand2->value.number == 0) {
			operator->value.number = 0;
			operator->isNumber = 1;
			operator->isTree = 0;
			operator->isOperand = 1;
			operator->left = NULL;
			operator->right = NULL;
			push(operand_stack, &operator->elem);
		    }

		    else {
			operator->isTree = 1;
			operator->left = operand1;
			operator->right = operand2;
			push(operand_stack, &operator->elem);
		    }
		}
		
		else if (operator->value.character == '/') {

		    if (operand2->value.number == 0) {
			fprintf(stderr, "Division by zero error\n");
			exit(1);
		    }
		    
		    else if (operand1->value.number == 0) {
			operator->value.number = 0;
			operator->isNumber = 1;
			operator->isTree = 0;
			operator->isOperand = 1;
			operator->left = NULL;
			operator->right = NULL;
			push(operand_stack, &operator->elem);
		    }		    
		    
		    else if (operand2->value.number == 1) {
			operator->value.character = operand1->value.character;
			operator->isNumber = 0;
			operator->isTree = 0;
			operator->isOperand = 1;
			operator->left = NULL;
			operator->right = NULL;
			push(operand_stack, &operator->elem);
		    }

		    else if (operand1->value.character == operand2->value.character) {
			operator->value.number = 1;
			operator->isNumber = 1;
			operator->isTree = 0;
			operator->isOperand = 1;
			operator->left = NULL;
			operator->right = NULL;
			push(operand_stack, &operator->elem);
		    }
		    
		    else {
			operator->isTree = 1;
			operator->left = operand1;
			operator->right = operand2;
			push(operand_stack, &operator->elem);
		    }
		}
		
		else if (!operand1->isNumber && !operand2->isNumber) {
		    
		    if (operand1->value.character == operand2->value.character) {
			
			if (operator->value.character == '-') {
			    operator->value.number = 0;
			    operator->isNumber = 1;
			    operator->isTree = 0;
			    operator->isOperand = 1;
			    operator->left = NULL;
			    operator->right = NULL;
			    push(operand_stack, &operator->elem);
			}
			
			else {
			    operator->isTree = 1;
			    operator->left = operand1;
			    operator->right = operand2;
			    push(operand_stack, &operator->elem);
			}
		    }
		    
		    else {
			operator->isTree = 1;
			operator->left = operand1;
			operator->right = operand2;
			push(operand_stack, &operator->elem);
		    }
		}
		
		else {
		    operator->isTree = 1;
		    operator->left = operand1;
		    operator->right = operand2;
		    push(operand_stack, &operator->elem);
		}
	    }
	}

	else {
	    operator->isTree = 1;
	    operator->left = operand1;
	    operator->right = operand2;
	    push(operand_stack, &operator->elem);
	}
    }
    
    else {
	operator->isTree = 1;
	operator->left = operand1;
	operator->right = operand2;
	push(operand_stack, &operator->elem);
    }
}

int operator_rank(char operator) {
    if (operator == OPEN_BRACKET)
	return 0;
    else if (operator == CLOSED_BRACKET)
	return 0;
    else if (operator == DIVISION)
	return 4;
    else if (operator == MULTIPLICATION)
	return 3;
    else if (operator == ADDITION)
	return 2;
    else if (operator == SUBTRACTION)
	return 1;

    return -1;
}

int precedence_calculator(char operator1, char operator2) {
    int op1rank = operator_rank(operator1);
    int op2rank = operator_rank(operator2);
    return (op1rank >= op2rank ? 1 : 0);
}

void reduce(struct node *root) {

    // constant folding
    constant_folding(root);
}

void constant_folding(struct node *root) {

    if (root == NULL || root->left == NULL || root->right == NULL)
	return;

    if (root->left->isOperand && root->left->isNumber == 1 && root->right->isOperand && root->right->isNumber == 1 && !root->isOperand) {

	char operator = root->value.character;
	int operand1 = root->left->value.number;
	int operand2 = root->right->value.number;

	int total;
	if (operator == '+')
	    total = operand1 + operand2;
	else if (operator == '-')
	    total = operand1 - operand2;
	else if (operator == '*')
	    total = operand1 * operand2;
	else if (operator == '/')
	    total = operand1 / operand2;

	root->isOperand = 1;
	root->isNumber = 1;
	root->isRightLeaf = 0;
	root->value.number = total;
	root->left = NULL;
	root->right = NULL;
	root->isRightLeaf = 0;

	printf("%d %c %d = %d\n", operand1, operator, operand2, total);
    }

    reduce(root->left);
    reduce(root->right);
}

void preorder_traversal(struct node *root) {    

    if (root == NULL)
	return;

    if (root->isOperand) {
	if (root->isNumber)
	    printf("%d ", root->value.number);
	else
	    printf("%c ", root->value.character);
    }

    else
	printf("( %c ", root->value.character);

    preorder_traversal(root->left);
    preorder_traversal(root->right);

    if (root->isRightLeaf)
	printf(") ");
}

void free_tree(struct node *root) {
    if (root == NULL)
	return;

    free_tree(root->right);
    free_tree(root->left);
    free(root);
}

void test_stack() {

    struct stack test_stack;
    stack_init(&test_stack);

    assert(count(&test_stack) == 0);
    assert(isEmpty(&test_stack) == 1);
    
    struct node *first_node = (struct node *)malloc(sizeof(struct node));
    first_node->value.number = 5;
    first_node->isOperand = 1;
    push(&test_stack, &first_node->elem);

    assert(count(&test_stack) == 1);
    assert(isEmpty(&test_stack) == 0);

    struct stack_elem *top = peek(&test_stack);
    struct node *converted_node = stack_entry(top, struct node, elem);
    assert(converted_node->value.number == 5);
    assert(converted_node->isOperand == 1);

    struct node *second_node = (struct node *)malloc(sizeof(struct node));
    second_node->value.character = 'z';
    second_node->isOperand = 0;
    push(&test_stack, &second_node->elem);

    assert(count(&test_stack) == 2);
    assert(isEmpty(&test_stack) == 0);

    top = peek(&test_stack);
    converted_node = stack_entry(top, struct node, elem);
    assert(converted_node->value.character == 'z');
    assert(converted_node->isOperand == 0);

    second_node = NULL;
    struct stack_elem *popped = pop(&test_stack);
    second_node = stack_entry(popped, struct node, elem);
    free(second_node);
    
    assert(count(&test_stack) == 1);
    assert(isEmpty(&test_stack) == 0);
    
    first_node = NULL;
    popped = pop(&test_stack);
    first_node = stack_entry(popped, struct node, elem);
    free(first_node);
    
    assert(count(&test_stack) == 0);
    assert(isEmpty(&test_stack) == 1);
}
