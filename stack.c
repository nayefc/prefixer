/*
 *
 * Created on: 6th July 2012
 * Author: Nayef Copty
 *
 */

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void push(struct node * node) {
    node->next = top;
    top = node;
}

struct node * pop() {

    if (isEmpty() == 1)
	return NULL;

    else {
	struct node *node = top;
	top = top->next;
	return node;
    }
}

struct node * peek() {

    if (isEmpty() == 1)
	return NULL;

    else
	return top;
}

int count() {

    if (isEmpty() == 1)
	return 0;

    else {
	struct node *temp;
	temp = top;

	int count = 0;
	while (temp != NULL) {
	    count++;
	    temp = temp->next;
	}

	return count;
    }
}

int isEmpty() {
    
    if (top == NULL)
	return 1;

    else
	return 0;
}

void clear() {

    if (isEmpty() == 0) {

	struct node *node;
	node = top;

	while (node != NULL) {
	    free(node);
	    node = node->next;
	}

	top = NULL;
    }
}

void test() {

    assert(top == NULL);
    assert(isEmpty() == 1);
    assert(count() == 0);

    assert(pop() == NULL);
    assert(peek() == NULL);

    struct node *node;
    node = (struct node *)malloc(sizeof(struct node));
    assert(node != NULL);
    node->character = 'c';
    push(node);
    assert(peek()->character == 'c');
    assert(isEmpty() == 0);
    assert(count() == 1);

    struct node *popped = pop();
    assert(popped->character == 'c');
    free(popped);
    assert(isEmpty() == 1);
    assert(count() == 0);

    ///////////////////////////

    node = (struct node *)malloc(sizeof(struct node));
    assert(node != NULL);
    node->character = '+';
    push(node);
    assert(peek()->character == '+');
    assert(isEmpty() == 0);
    assert(count() == 1);

    struct node *second_node;
    second_node = (struct node *)malloc(sizeof(struct node));
    assert(second_node != NULL);
    second_node->number = 1;
    push(second_node);
    assert(peek()->number == 1);
    assert(isEmpty() == 0);
    assert(count() == 2);

    struct node *third_node;
    third_node = (struct node*)malloc(sizeof(struct node));
    assert(third_node != NULL);
    third_node->character = '/';
    push (third_node);
    assert(peek()->character == '/');
    assert(isEmpty() == 0);
    assert(count() == 3);

    struct node *fourth_node;
    fourth_node = (struct node *)malloc(sizeof(struct node));
    assert(fourth_node != NULL);
    fourth_node->number = 5;
    push(fourth_node);
    assert(peek()->number == 5);
    assert(isEmpty() == 0);
    assert(count() == 4);

    popped = pop();
    assert(popped->number == 5);
    free(popped);
    assert(isEmpty() == 0);
    assert(count() == 3);
    assert(peek()->character == '/');

    popped = pop();
    assert(popped->character == '/');
    free(popped);
    assert(isEmpty() == 0);
    assert(count() == 2);
    assert(peek()->number == 1);

    popped = pop();
    assert(popped->number == 1);
    free(popped);
    assert(isEmpty() == 0);
    assert(count() == 1);
    assert(peek()->character == '+');

    popped = pop();
    assert(popped->character == '+');
    free(popped);
    assert(isEmpty() == 1);
    assert(count() == 0);
    
    assert(pop() == NULL);
    assert(peek() == NULL);

    assert(top == NULL);

    ///////////////////////////

    node = (struct node *)malloc(sizeof(struct node));
    node->number = 1412;
    push(node);
    assert(isEmpty() == 0);
    assert(count() == 1);
    assert(top != NULL);
    clear();    
    assert(top == NULL);
    assert(isEmpty() == 1);
    assert(count() == 0);
}
