#include <stdio.h>
#include <stdlib.h>

#include "brainfuck.h"

struct node *push(struct node *head, struct instruction *value) {
    struct node *temp = (struct node *) malloc(sizeof(struct node));

    // TODO: check temp is NULL

    temp->value = value;
    temp->next = head;
    head = temp;

    return head;
}

struct node *pop(struct node *head, struct instruction **item) {
    struct node *temp = head;

    *item = head->value;
    head = head->next;

    free(temp);

    return head;
}

int is_empty(struct node *head) {
    return head == NULL ? 1 : 0;
}

int is_simple_token(char token) {
    return token == NEXT_TOKEN
            || token == PREV_TOKEN
            || token == INC_TOKEN
            || token == DEC_TOKEN
            || token == WRITE_TOKEN
            || token == READ_TOKEN;
}

int is_loop_token(char token) {
    return token == LOOP_START_TOKEN
            || token == LOOP_END_TOKEN;
}

int is_valid_token(char token) {
    return is_simple_token(token) || is_loop_token(token);
}

void log_loop_error(struct instruction *item) {
    if (item == NULL) return;
    fprintf(stderr, "Error [%lu:%lu]\n", item->line, item->position);
}

struct instruction *create(
        char token,
        unsigned long line,
        unsigned long position,
        struct instruction *current) {

    struct instruction *result;

    if (!is_loop_token(token) && current != NULL && current->operation == token) {
        current->count++;
        result = current;
    } else {
        result = (struct instruction *) malloc(sizeof(struct instruction));

        // TODO: check temp is NULL

        result->operation = token;
        result->count = 1;
        result->line = line;
        result->position = position + 1;
    }

    return result;
}

struct instruction *parse(char *source) {
    int result = 1;

    unsigned long line = 1;
    unsigned long position;

    struct node *stack = NULL;

    struct instruction *head = NULL, *current = NULL, *temp;

    for (position = 0; source[position]; position++) {
        char token = source[position];

        if (token == '\n') line++;
        if (!is_valid_token(token)) continue;

        temp = create(token, line, position, current);

        if (head == NULL) {
            head = temp;
            current = head;
        } else {
            current->next = temp;
            current = temp;
        }

        if (current->operation == LOOP_START_TOKEN) {
            stack = push(stack, current);
        }
    }

    current = head;

    while (current) {

        if (current->operation == LOOP_END_TOKEN) {
            struct instruction *loop = NULL;

            if (is_empty(stack)) {
                result = 0;
                log_loop_error(current);
            } else {
                stack = pop(stack, &loop);
                loop->loop = loop->next;
                loop->next = current->next;
                current->next = loop;
            }
        }

        current = current->next;
    }

    while (!is_empty(stack)) {
        result = 0;
        struct instruction *loop = NULL;
        stack = pop(stack, &loop);
        log_loop_error(loop);
    }

    return result ? head : NULL;
}

unsigned long next_operation(unsigned long index, unsigned long count) {
    return (index + count) % MAX_ARRAY_LENGTH;
}

unsigned long prev_operation(unsigned long index, unsigned long count) {
    if ((index - count) < 0) {
        return MAX_ARRAY_LENGTH + index - count;
    } else {
        return index - count;
    }
}

void write_operation(char *array, unsigned long index, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        printf("%c", array[index]);
    }
}

void read_operation(char *array, unsigned long index, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        array[index] = (char) getchar();
    }
}

void interpret(struct instruction *head) {
    struct instruction *step;
    char array[MAX_ARRAY_LENGTH];
    unsigned long index = 0;

    step = head;

    while (step) {
        char operation = step->operation;

        if (operation == LOOP_START_TOKEN) {
            if (array[index] == 0) {
                step = step->next;
            } else {
                step = step->loop;
            }

            continue;
        }

        unsigned long count = step->count;

        switch (operation) {
            case NEXT_TOKEN:
                index = next_operation(index, count);
                break;
            case PREV_TOKEN:
                index = prev_operation(index, count);
                break;
            case INC_TOKEN:
                array[index] += count;
                break;
            case DEC_TOKEN:
                array[index] -= count;
                break;
            case WRITE_TOKEN:
                write_operation(array, index, count);
                break;
            case READ_TOKEN:
                read_operation(array, index, count);
                break;
            default: break;
        }

        step = step->next;
    }
}

void run(char *source) {
    struct instrucation *head = NULL;

    head = (struct instrucation *) parse(source);

    if (head == NULL) return;

    interpret(head);
}