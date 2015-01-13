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

int is_loop_token(char token) {
    return token == LOOP_START_TOKEN
            || token == LOOP_END_TOKEN;
}

int is_valid_token(char token) {
    return token == NEXT_TOKEN
            || token == PREV_TOKEN
            || token == INC_TOKEN
            || token == DEC_TOKEN
            || token == WRITE_TOKEN
            || token == READ_TOKEN
            || is_loop_token(token);
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
    unsigned long position = 0;

    unsigned long index;

    struct node *stack = NULL;

    struct instruction *head = NULL, *current = NULL, *temp;

    for (index = 0; source[index]; index++, position++) {
        char token = source[index];

        if (token == '\n') {
            line++;
            position = 0;
        }

        if (!is_valid_token(token)) continue;

        temp = create(token, line, position, current);

        if (head == NULL) {
            head = temp;
            current = head;
        } else {
            current->next = temp;
            current = temp;
        }

        switch (current->operation) {
        case LOOP_START_TOKEN: {
            stack = push(stack, current);
        } break;
        case LOOP_END_TOKEN: {
            struct instruction *loop = NULL;

            if (is_empty(stack)) {
                result = 0;
                log_loop_error(current);
            } else {
                stack = pop(stack, &loop);
                current->loop = loop;
                loop->loop = current;
            }
        } break;
        default: break;
        }
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
                step = step->loop->next;
            } else {
                step = step->next;
            }

            continue;
        } else if (operation == LOOP_END_TOKEN) {
            step = step->loop;

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