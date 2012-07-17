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
    int isNumber;
    struct stack_elem elem;
};

struct grouped_node {
    struct node *node1;
    struct node *node2;
    struct node *node3;
    struct stack_elem elem;
};

void test_stack();
int precedence_calculator(char operator1, char operator2);

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
    //    unsigned long int str_len;
    //    str_len = strlen(buf) - 1;
    
    int i;
    for (i = 0; i < str_len; i++) {//while (*p != '\0') {

	if (*p == ' ') {
	    p++;
	    continue;
	}

	int isDigit;
	int digit;
	if (isdigit(*p)) {
	    isDigit = 1;
	    sscanf(p, "%d", &digit);
	}
	    
	else
	    isDigit = 0;

	// operand
	if (isDigit) {
	    struct node *node = (struct node *)malloc(sizeof(struct node));
	    int x = 0;
	    sscanf(p, "%d", &x);
	    node->number = x;
	    node->isNumber = 1;
	    push(&operand_stack, &node->elem);
	    
	    int digit_length = log10(x) + 1;
	    int i;
	    for (i = 0; i < digit_length; i++)
		p++;
	    continue;
	}

	else {

	    struct node *operator_top;
	    if (!isEmpty(&operator_stack)) {
		struct stack_elem *e = peek(&operator_stack);
		operator_top = stack_entry(e, struct node, elem);
	    }
	    
	    if (*p == OPEN_BRACKET || isEmpty(&operator_stack) || precedence_calculator(*p, operator_top->character)) {
		struct node *node = (struct node *)malloc(sizeof(struct node));
		node->character = *p;
		node->isNumber = 0;
		push(&operator_stack, &node->elem);
		
		p++;
		continue;
	    }

	    else if (*p == CLOSED_BRACKET) {

		while (operator_top->character != '(') {

		    struct stack_elem *operator_e = pop(&operator_stack);
		    struct node *operator = stack_entry(operator_e, struct node, elem);
		    
		    struct stack_elem *e1 = pop(&operand_stack);
		    struct node *operand2 = stack_entry(e1, struct node, elem);
		    
		    struct stack_elem *e2 = pop(&operand_stack);
		    struct node *operand1 = stack_entry(e2, struct node, elem);

		    push(&operand_stack, &operator->elem);
		    push(&operand_stack, &operand1->elem);
		    push(&operand_stack, &operand2->elem);

		    struct stack_elem *e = peek(&operator_stack);
		    operator_top = stack_entry(e, struct node, elem);
		}

		struct stack_elem *open_bracket_e = pop(&operator_stack);
		struct node *open_bracket = stack_entry(open_bracket_e, struct node, elem);
		free(open_bracket);
	    }

	    else if (precedence_calculator(operator_top->character, *p) == 0) {
		
		while (!isEmpty(&operator_stack) && precedence_calculator(operator_top->character, *p) == 0) {

		    struct stack_elem *operator_e = pop(&operator_stack);
		    struct node *operator = stack_entry(operator_e, struct node, elem);
		    
		    struct stack_elem *e1 = pop(&operand_stack);
		    struct node *operand2 = stack_entry(e1, struct node, elem);
		    
		    struct stack_elem *e2 = pop(&operand_stack);
		    struct node *operand1 = stack_entry(e2, struct node, elem);
		    
		    push(&operand_stack, &operator->elem);
		    push(&operand_stack, &operand1->elem);
		    push(&operand_stack, &operand2->elem);
		}

		struct node *node = (struct node *)malloc(sizeof(struct node));
		node->character = *p;
		node->isNumber = 0;
		push(&operator_stack, &node->elem);
	    }
	}
    }

    while (!isEmpty(&operator_stack)) {
	struct stack_elem *operator_e = pop(&operator_stack);
	struct node *operator = stack_entry(operator_e, struct node, elem);
	
	struct stack_elem *e1 = pop(&operand_stack);
	struct node *operand2 = stack_entry(e1, struct node, elem);
	
	struct stack_elem *e2 = pop(&operand_stack);
	struct node *operand1 = stack_entry(e2, struct node, elem);
	
	push(&operand_stack, &operator->elem);
	push(&operand_stack, &operand1->elem);
	push(&operand_stack, &operand2->elem); 
    }
    
    // operand stack is final expression
    printf("Stack count: %d\n", count(&operand_stack));
    
    /*
     * Iterate stack
     */
    struct stack_elem *e = peek(&operand_stack);
    while (e != NULL) {
	
	struct node *node = stack_entry(e, struct node, elem);
	if (node->isNumber)
	    printf("%d ", node->number);
	else
	    printf("%c ", node->character);
	e = e->next;
    }
    printf("\n");
    
    return 0;
}

int operator_rank(char operator) {
    if (operator == OPEN_BRACKET)
	return 5;
    else if (operator == CLOSED_BRACKET)
	return 5;
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
