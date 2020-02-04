/***********************************************************************
 * fixme.c
 * Devin J. Pohly, Penn State
 * See http://www.cse.psu.edu/~djp284/cmpsc311-s14/slides/
 * 23-debugging-practice.pdf
 *
 * Revised by Tom Kelliher, Goucher College
 *
 * This program is a gdb practice lab.  It defines a pointer-based
 * data structure for a singly-linked list and a few functions for
 * manipulating the list.  The main() function exercises the functions.
 *
 * To compile the program with debugging symbols:
 *
 *    gcc -g -o fixme fixme.c
 *
 * To run the program on its own:
 *
 *    ./fixme
 *
 * To load the program, after compiling it, into gdb:
 *
 *    gdb fixme
 *
 * Refer to gdb documentation for debugging a program loaded into gdb.
 *
 * Now for the debugging detective work ---
 * This program contains five bugs.  Find and fix all five.  Four
 * of the bugs are relatively simple --- change or add a line or
 * two of code.  One of the bugs is due to a design problem; it will
 * require you to make a design change that will probably have
 * consequences for one or more of the list manipulation functions.
 ***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// Definition for an element of the singly-linked list of ints.
// next is a pointer to the next element of the list.  It should be
// NULL for the final list element.
typedef struct int_list {
    struct int_list *next;
    int value;
} int_list;


// Function prototypes.  The C compiler is a single pass compiler.
// Therefore, functions must either be declared, as is done here, or
// defined before they are used.  Parameter names are optional in
// function prototypes.
// In C, the variable declaration syntax
//    bar *foo;
// defines foo to be a pointer variable.  As such, it is used to store
// the memory address of a variable of type bar.
int list_add(int_list *, int);
int list_contains(int_list *, int);
void list_remove(int_list *, int_list *);
int list_remove_first(int_list *, int);
void list_print(int_list *);


// Set up a linked list of primes at compile-time.
// In C, the & operator is the address-of operator.  It returns the
// memory address of a variable.
int_list list6 = {.value = 17, .next = NULL};
int_list list5 = {.value = 13, .next = &list6};
int_list list4 = {.value = 11, .next = &list5};
int_list list3 = {.value = 7, .next = &list4};
int_list list2 = {.value = 5, .next = &list2};
int_list list1 = {.value = 3, .next = &list2};
int_list primes = {.value = 2, .next = &list1};


// Tests all of our functions.
int main(int argc, char **argv)
{
    // Set up a linked list of evens at run-time.
    int_list even = {.next = NULL, .value = 0};
    list_add(&even, 2);
    list_add(&even, 4);
    list_add(&even, 6);
    list_add(&even, 8);
    // Paraphrasing "Star Wars," this isn't the bug you're looking for.
    list_add(&even, 9);
    list_add(&even, 10);

    printf("Even list: ");
    list_print(&even);

    // Set up a linked list of odds at run-time.
    int_list odd;
    list_add(&odd, 1);
    list_add(&odd, 3);
    list_add(&odd, 5);
    list_add(&odd, 7);

    printf("Odd list: ");
    list_print(&odd);

    list_remove_first(&even, 9);

    printf("Fixed even list: ");
    list_print(&even);

    printf("Returned: %d\n", list_remove_first(&even, 42));
    list_remove_first(&even, 0);

    printf("Even list now: ");
    list_print(&even);

    if (list_contains(&primes, 3))
        printf("primes contains 3\n");
    else
        printf("primes doesn't contain 3\n");

    if (list_contains(&primes, 6))
        printf("primes contains 6\n");
    else
        printf("primes doesn't contain 6\n");

    return 0;
}


// Adds an item (argument n) to the end of a list (argument list).
// Returns 0.
int list_add(int_list *list, int n)
{
    int_list *end = list;

    // Find the end of the list.
    // In C, a condition is true if it evaluates to a non-zero value.
    // The NULL pointer has a value of 0.  This condition is equivalent to
    // 'list != NULL', but this is the way most experienced C programmers
    // write their code.
    while (list) {
        end = list;
        // Advance to the next element of the list.
        // Note the use of the -> operator.  This operator de-references a
        // pointer to a struct variable (the operand on the left) and then
        // accesses a field of the struct (the operand on the right).
        // The operator is shorthand for (*list).next
        list = list->next;
    }

    // Allocate space for a new list element, initialize it, and
    // add it to the end of the list.
    int_list *newnode = malloc(sizeof(int_list));
    newnode->value = n;
    newnode->next = NULL;
    end->next = newnode;
    return 0;
}


// Returns 1 if a list (argument list) contains the given value
// (int n), 0 otherwise.
int list_contains(int_list *list, int n)
{
    while (list) {
        if (list->value == n)
            return 1;
        list = list->next;
    }
    return 0;
}


// Removes a node (parameter node) from a linked list.  prev must point to
// the previous node in the list.
// No return value.
void list_remove(int_list *node, int_list *prev)
{
    // prev must be the node before node.
    // assert() will terminate the program if its condition is false.
    assert(prev->next == node);

    prev->next = node->next;
    free(node);
}


// Removes the first occurrence of a number (parameter n) from a list
// (parameter list).
// Returns 0.
int list_remove_first(int_list *list, int n)
{
    // Search for the list element n.  Remove it, when found.
    int_list *prev = list;
    while (list) {
        if (list->value == n) {
            list_remove(list, prev);
            return 0;
        }
        list = list->next;
    }

    // I think we should never get here.
    assert(0);
}


// Print the elements of a list (parameter list).
// Returns nothing.
void list_print(int_list *list)
{
    while (list) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}
