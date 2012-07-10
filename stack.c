/*
 *
 * Created on: 9th July 2012
 * Author: Nayef Copty
 *
 */

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void stack_init(struct stack *stack) {
    assert(stack != NULL);
    stack->top = NULL;
}

void push(struct stack *stack, struct stack_elem *elem) {
    elem->next = stack->top;
    stack->top = elem;
}

struct stack_elem * pop(struct stack *stack) {

    if (isEmpty(stack) == 1)
	return NULL;

    else {
	struct stack_elem *node = stack->top;
	stack->top = stack->top->next;
	return node;
    }
}

struct stack_elem * peek(struct stack *stack) {

    if (isEmpty(stack) == 1)
	return NULL;

    else
	return stack->top;
}

int count(struct stack *stack) {

    if (isEmpty(stack) == 1)
	return 0;

    else {
	struct stack_elem *temp;
	temp = stack->top;

	int count = 0;
	while (temp != NULL) {
	    count++;
	    temp = temp->next;
	}

	return count;
    }
}

int isEmpty(struct stack *stack) {
    
    if (stack->top == NULL)
	return 1;

    else
	return 0;
}

void clear(struct stack *stack) {

    if (isEmpty(stack) == 0) {

	struct stack_elem *node;
	node = stack->top;

	while (node != NULL) {
	    node = stack->top->next;
	    stack->top = NULL;
	    stack->top = node;
	}

	stack->top = NULL;
    }
}
