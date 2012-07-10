#include <stdio.h>
#include <stdlib.h>
#include "stack.c"
#include <assert.h>

#define OPEN_BRACKET "("
#define CLOSE_BRACKET ")"
#define ADDITION "+"
#define SUBTRACTION "-"
#define MULTIPLICATION "*"
#define DIVISION "/"

struct node {
    double number;
    char character;
    int isNumber;
    struct stack_elem elem;
};

struct grouped_node {
    struct node *node1;
    struct node *node2;
    struct node *node3;
};

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

int main(int argc, char *argv[]) {

    test_stack();
    
    struct stack stack;
    stack_init(&stack);

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
	printf("Please pass in a file with an expression\n");
	exit(1);
    }

    char reading;
    while (!feof(fp)) {

	fscanf(fp, "%c", &reading);
	
	// skip if whitspace
	if (reading == ' ')
	    continue;
	
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (isdigit(reading)) {
	    double number = reading - '0';
	    node->number = number;
	    node->isNumber = 1;
	}
	
	else {
	    node->character = reading;
	    node->isNumber = 0;
	}

	push(&stack, &node->elem);

	if (count(&stack) == 3) {

	    struct

	    struct stack_elem *e = peek(&stack);
	    struct node *node = stack_entry(e, struct node, elem);
	    
	    if (node->isNumber)
		printf("count is 3 with top %f", node->number);
	    else
		printf("count is 3 with top %c", node->character);
	}
    }
    
    return 0;
}
