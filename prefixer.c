#include "stack.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

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

    /*struct stack stack;
    stack_init(&stack);
    struct stack prefix_stack;
    stack_init(&prefix_stack);*/
    
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
    if (fgets(buf, 20, fp) == NULL) {
	printf("Error in file parsing.\n");
	exit(1);
    }    
    char *p;
    p = buf;

    while (*p != '\n') {

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

	if (!isDigit) {

	    if (*p == OPEN_BRACKET) {
		struct node *node = (struct node *)malloc(sizeof(struct node));
		node->character = OPEN_BRACKET;
		node->isNumber = 0;						      
		push(&operator_stack, &node->elem);
		printf("Pushed %c\n", node->character);
		
		p++;
		continue;
	    }
	    
	    else if (*p == CLOSED_BRACKET) {
		
		for (;;) {
		    
		    struct stack_elem *e = pop(&operator_stack);
		    struct node *operator = stack_entry(e, struct node, elem);
		    
		    if (operator->character != OPEN_BRACKET) {
			struct stack_elem *e1 = pop(&operand_stack);
			struct node *operand2 = stack_entry(e, struct node, elem);
			
			struct stack_elem *e2 = pop(&operand_stack);
			struct node *operand1 = stack_entry(e, struct node, elem);
			
			push(&operand_stack, &operator->elem);
			push(&operand_stack, &operand1->elem);
			push(&operand_stack, &operand2->elem);
			
			printf("Pushed %c - ", operator->character);
			printf("Pushed %d - ", operand1->number);
			printf("Pushed %d\n", operand2->number);
			
			p++;
			continue;
		    }
		}
	    }
	    
	    else if (*p == ADDITION || *p == SUBTRACTION || *p == MULTIPLICATION || *p == DIVISION) {

		struct stack_elem *e = pop(&operator_stack);
		if (e != NULL) {

		    printf("HI");
		    
		    struct node *operator = stack_entry(e, struct node, elem);
		    
		    while (precedence_calculator(operator->character, *p)) {
			
			struct stack_elem *e1 = pop(&operand_stack);
			struct node *operand2 = stack_entry(e, struct node, elem);
			
			struct stack_elem *e2 = pop(&operand_stack);
			struct node *operand1 = stack_entry(e, struct node, elem);
			
			push(&operand_stack, &operator->elem);
			push(&operand_stack, &operand1->elem);
			push(&operand_stack, &operand2->elem);
			
			printf("Pushed %c - ", operator->character);
			printf("Pushed %d - ", operand1->number);
			printf("Pushed %d\n", operand2->number);
		    }
		    
		    else {
			push(&operator_stack, &operator->elem);
			struct node *node = (struct node *)malloc(sizeof(struct node));
			node->character = *p;
			node->isNumber = 0;
			push(&operator_stack, &node->elem);
			
			printf("Pushed %c\n", node->character);
		    }
		}

		else {
		    
		}
		
		p++;
		continue;
	    }

	}
	
	else {
	    struct node *node = (struct node *)malloc(sizeof(struct node));
	    int x = 0;
	    sscanf(p, "%d", &x);
	    node->number = x;
	    node->isNumber = 1;
	    push(&operand_stack, &node->elem);
	    printf("Pushed %d\n", node->number);
	    
	    int digit_length = log10(x) + 1;
	    int i;
	    for (i = 0; i < digit_length; i++)
		p++;
	    continue;
	}
    }
    
    // operand stack is final expression
    printf("Count: %d\n---------\n", count(&operand_stack));
    
    /*
     * Iterate stack
     */
    struct stack_elem *e = peek(&operand_stack);
    while (e != NULL) {

	struct node *node = stack_entry(e, struct node, elem);
	if (node->isNumber)
	    printf("%d\n", node->number);
	else
	    printf("%c\n", node->character);
	e = e->next;
    }
    

    return 0;
}
	
	
	
	
	
	
	
/*	
	//	fscanf(fp, "%c", buf);
	
	// skip if whitspace
	if (buf[0] == ' ')
	    continue;
	
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (buf[0] == ADDITION || buf[0] == SUBTRACTION || buf[0] == DIVISION || buf[0] == MULTIPLICATION) {
	    node->character = buf[0];
	    node->isNumber = 0;
	}
	
	else {
	    int number = atoi(buf);
	    node->number = number;
	    node->isNumber = 1;
	}
	
	//	if (buf[0] == ADDITION || buf[0] == SUBTRACTION || buf[0] DIVISION || buf[0] == MULTIPLICATION) {
	//  //if (isdigit(reading)) {
	    //  int number = atoi(buf);
	    //node->number = number;
	// node->isNumber = 1;
	//}
	
	//else {
	//   node->character = reading;
	//   node->isNumber = 0;
	//	}

	push(&stack, &node->elem);

	if (count(&stack) == 3) {

	    struct stack_elem *e = pop(&stack);
	    struct node *node = stack_entry(e, struct node, elem);
	    double operand1 = node->number;
	    free(node);

	    e = pop(&stack);
	    node = stack_entry(e, struct node, elem);
	    char operator = node->character;
	    free(node);

	    e = pop(&stack);
	    node = stack_entry(e, struct node, elem);
	    double operand2 = node->number;
	    free(node);

	    printf("Stack contains: %c %f %f\n", operator, operand1, operand2);

	    struct node *node1 = (struct node *)malloc(sizeof(struct node));
	    node1->character = operator;
	    node1->isNumber = 0;

	    struct node *node2 = (struct node *)malloc(sizeof(struct node));
	    node2->number = operand1;
	    node2->isNumber = 1;

	    struct node *node3 = (struct node *)malloc(sizeof(struct node));
	    node3->number = operand2;
	    node3->isNumber = 1;

	    struct grouped_node *grouped_node = (struct grouped_node *)malloc(sizeof(struct grouped_node));
	    grouped_node->node1 = node1;
	    grouped_node->node2 = node2;
	    grouped_node->node3 = node3;

	    push(&prefix_stack, &grouped_node->elem);

	    // clear stack
	    struct stack cleared_nodes = clear(&stack);
	    struct stack_elem *top = peek(&stack);
	    while (top != NULL) {		
		struct  stack_elem *elem = pop(&stack);
		struct node *node = stack_entry(e, struct node, elem);
		free(node);
		top = peek(&stack);
	    }
	}
    }

    struct stack_elem *top = peek(&prefix_stack);
    struct grouped_node *grouped_node = stack_entry(top, struct grouped_node, elem);
    printf("%c %d %d", grouped_node->node1->character, grouped_node->node2->number, grouped_node->node3->number);
    
    
    return 0;
    }*/

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

    return (op1rank >= op2rank ? 1 : 0);
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
