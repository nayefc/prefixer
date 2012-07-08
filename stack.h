/*
 *
 * Created on: 6th July 2012
 * Author: Nayef Copty
 *
 */

#ifndef _STACK_H_
#define _STACK_H_

/*
 * A stack that stores a node that holds a number and a character
 */

struct node {
    double number;
    char character;
    struct node *next;
};

struct node *top;

/* pushes a dynamically allocated node */
void push(struct node *);

/* pop an allocated node - must free yourself */
struct node * pop();

struct node * peak();
int count();
int isEmpty();
void clear();
void test();

#endif /* stack.h */
