#include "stack.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#define OPEN_BRACKET '('
#define CLOSED_BRACKET ')'
#define ADDITION '+'
#define SUBTRACTION '-'
#define MULTIPLICATION '*'
#define DIVISION '/'

struct node {
    int number;
    char character;
    struct node *left;
    struct node *right;
    int isNumber;
    int isTree;
    int isRightLeaf;
    struct stack_elem elem;
};

int isOperator(char p);
void build_expression(struct stack *operator_stack, struct stack *operand_stack);
int precedence_calculator(char operator1, char operator2);
void preorder_traversal(struct node *root);
void test_stack();

int main(int argc, char *argv[]) {

    test_stack();
    
    struct stack operator_stack;
    stack_init(&operator_stack);

    struct stack operand_stack;
    stack_init(&operand_stack);

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
	printf("Please pass in a file with an expression\n");
	exit(1);
    }

    char buf[50];
    if (fgets(buf, 50, fp) == NULL) {
	printf("Error in file parsing.\n");
	exit(1);
    }    
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
	    if (*p == OPEN_BRACKET || isEmpty(&operator_stack) || precedence_calculator(*p, operator_top->character)) {

		struct node *node = (struct node *)malloc(sizeof(struct node));
		if (node == NULL) {
		    printf("Out of memory\n");
		    exit(1);
		}
		
		node->character = *p;
		node->isNumber = 0;
		node->isTree = 0;
		push(&operator_stack, &node->elem);

		p++;
		continue;
	    }

	    else if (*p == CLOSED_BRACKET) {
		
		operator_top = stack_entry(peek(&operator_stack), struct node, elem);
		
		while (operator_top->character != '(') {
		    build_expression(&operator_stack, &operand_stack);
		    operator_top = stack_entry(peek(&operator_stack), struct node, elem);
		}


		struct stack_elem *open_bracket_e = pop(&operator_stack);
		struct node *open_bracket = stack_entry(open_bracket_e, struct node, elem);
		free(open_bracket);

		p++;
		continue;
	    }

	    // if stack top has higher precendence than scanned character
	    else if (precedence_calculator(operator_top->character, *p)) {
				
		while (operator_top != NULL && precedence_calculator(operator_top->character, *p)) {
		    build_expression(&operator_stack, &operand_stack);
		    if (isEmpty(&operator_stack))
			break;
		    struct stack_elem *e = pop(&operator_stack);
		    e = e->next;
		    operator_top = stack_entry(e, struct node, elem);
		}
		
		struct node *node = (struct node *)malloc(sizeof(struct node));
		if (node == NULL) {
		    printf("Out of memory\n");
		    exit(1);
		}
		
		node->character = *p;
		node->isNumber = 0;
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
		printf("Out of memory\n");
		exit(1);
	    }
	
	    int x = 0;
	    sscanf(p, "%d", &x);
	    node->number = x;
	    node->isNumber = 1;
	    node->isTree = 0;
	    push(&operand_stack, &node->elem);
	
	    int digit_length = log10(x) + 1;
	    int i;
	    for (i = 0; i < digit_length; i++)
		p++;

	    continue;
	}
    }
    
    while (!isEmpty(&operator_stack))
	build_expression(&operator_stack, &operand_stack);
    
    if (count(&operand_stack) > 1 && count(&operator_stack) > 0) {
	printf("Error in algorithm.\n");
	exit(1);
    };

    struct stack_elem *root_element = peek(&operand_stack);
    struct node *root = stack_entry(root_element, struct node, elem);

    preorder_traversal(root);
    printf("\n");

    
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

void build_expression(struct stack *operator_stack, struct stack *operand_stack) {

    struct stack_elem *operator_element = pop(operator_stack);
    struct node *operator = stack_entry(operator_element, struct node, elem);

    struct stack_elem *operand2_element = pop(operand_stack);
    struct node *operand2 = stack_entry(operand2_element, struct node, elem);
    operand2->isRightLeaf = 1;
    
    struct stack_elem *operand1_element = pop(operand_stack);
    struct node *operand1 = stack_entry(operand1_element, struct node, elem);
    operand1->isRightLeaf = 0;
    
    operator->isTree = 1;
    operator->left = operand1;
    operator->right = operand2;
    push(operand_stack, &operator->elem);
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
}

int precedence_calculator(char operator1, char operator2) {
    int op1rank = operator_rank(operator1);
    int op2rank = operator_rank(operator2);
    return (op1rank > op2rank ? 1 : 0);
}

void preorder_traversal(struct node *root) {    
    if (root == NULL)
	return;

    if (root->isNumber)
	printf("%d ", root->number);
    else
	printf("(%c ", root->character);
    preorder_traversal(root->left);
    preorder_traversal(root->right);

    if (root->isRightLeaf)
	printf(") ");
}

void test_stack() {

    struct stack test_stack;
    stack_init(&test_stack);

    assert(count(&test_stack) == 0);
    assert(isEmpty(&test_stack) == 1);
    
    struct node *first_node = (struct node *)malloc(sizeof(struct node));
    first_node->number = 5;
    first_node->character = 'c';    
    first_node->isNumber = 1;
    push(&test_stack, &first_node->elem);

    assert(count(&test_stack) == 1);
    assert(isEmpty(&test_stack) == 0);

    struct stack_elem *top = peek(&test_stack);
    struct node *converted_node = stack_entry(top, struct node, elem);
    assert(converted_node->character == 'c');
    assert(converted_node->number == 5);
    assert(converted_node->isNumber == 1);

    struct node *second_node = (struct node *)malloc(sizeof(struct node));
    second_node->number = 10;
    second_node->character = 'z';
    second_node->isNumber = 0;
    push(&test_stack, &second_node->elem);

    assert(count(&test_stack) == 2);
    assert(isEmpty(&test_stack) == 0);

    top = peek(&test_stack);
    converted_node = stack_entry(top, struct node, elem);
    assert(converted_node->character == 'z');
    assert(converted_node->number == 10);
    assert(converted_node->isNumber == 0);

    second_node = NULL;
    struct stack_elem *popped = pop(&test_stack);
    second_node = stack_entry(popped, struct node, elem);
    assert(converted_node->character == 'z');
    assert(converted_node->number == 10);
    assert(converted_node->isNumber == 0);
    
    assert(count(&test_stack) == 1);
    assert(isEmpty(&test_stack) == 0);

    clear(&test_stack);

    assert(count(&test_stack) == 0);
    assert(isEmpty(&test_stack) == 1);
}
