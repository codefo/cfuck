#include <stdio.h>
#include <stdlib.h>

#include "brainfuck.h"

void log_loop_error(struct step *item) {
    if (item == NULL) return;
    fprintf(stderr, "Error [%lu:%lu\n", item->line, item->position);
}

/*
 * Push step to stack
 */
struct stack *push(struct stack *head, struct step *value) {
    struct stack *temp = (struct stack *) malloc(sizeof(struct stack));

    // TODO: check temp is NULL

    temp->value = value;
    temp->next = head;
    head = temp;

    return head;
}

/*
 * Pop step from stack
 */
struct stack *pop(struct stack *head, struct step **item) {
    struct stack *temp = head;

    *item = head->value;
    head = head->next;

    free(temp);

    return head;
}

/*
 * Check token for filter source code
 */
int is_brainfuck_token(char token) {
    return token == NEXT_TOKEN
            || token == PREV_TOKEN
            || token == INC_TOKEN
            || token == DEC_TOKEN
            || token == WRITE_TOKEN
            || token == READ_TOKEN
            || token == LOOP_START_TOKEN
            || token == LOOP_END_TOKEN;
}

/*
 * Create step or return last step if is not a loop and same operations
 */
struct step *create(char token, struct step *last) {
    int is_not_loop = token != LOOP_START_TOKEN && token != LOOP_END_TOKEN;
    int is_repeating = last != NULL && last->operation == token;

    if (is_not_loop && is_repeating) {
        last->count += 1;
        return last;
    }

    struct step *result = (struct step *) malloc(sizeof(struct step));

    // TODO: check temp is NULL

    result->operation = token;
    result->count = 1;

    return result;
}

/*
 * Binding sibling loop steps
 */
int bind(struct stack **stack, struct step *step) {
    if (step->operation == LOOP_START_TOKEN) {
        *stack = push(*stack, step);

        return 0;
    }

    if (step->operation == LOOP_END_TOKEN) {
        if (*stack == NULL) return 1;

        struct step *sibling = NULL;
        *stack = pop(*stack, &sibling);
        step->loop = sibling;
        sibling->loop = step;
    }

    return 0;
}

/*
 * Append stack to errors if stack is not empty
 */
struct stack *append(struct stack *stack, struct stack *errors) {
    if (stack != NULL) {
        if (errors == NULL) return stack;

        errors->next = stack;
    }

    return errors;
}

struct state *parse(char *source) {
    unsigned long line = 1;
    unsigned long position = 1;

    struct stack *stack = NULL,
                 *errors = NULL;

    struct step *first = (struct step *) malloc(sizeof(struct step)),
                *current = first,
                *temp;

    for (unsigned long index = 0; source[index]; index++, ++position) {
        char token = source[index];

        if (token == '\n') {
            line++;
            position = 0;
        }

        if (!is_brainfuck_token(token)) continue;

        temp = create(token, current);

        temp->line = line;
        temp->position = position;

        current->next = temp;
        current = temp;

        if (bind(&stack, current)) {
            errors = push(errors, current);
        }
    }

    struct state *result = (struct state *) malloc(sizeof(struct state));

    result->errors = append(stack, errors);
    result->first = result->errors == NULL ? first : NULL;

    return result;
}

unsigned long next(unsigned long index, unsigned long count) {
    return (index + count) % MAX_ARRAY_LENGTH;
}

unsigned long prev(unsigned long index, unsigned long count) {
    if ((index - count) < 0) {
        return MAX_ARRAY_LENGTH + index - count;
    } else {
        return index - count;
    }
}

void write(char *tape, unsigned long index, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        printf("%c", tape[index]);
    }
}

void read(char *tape, unsigned long index, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        tape[index] = (char) getchar();
    }
}

void interpret(struct step *step) {
    char tape[MAX_ARRAY_LENGTH];
    unsigned long index = 0;

    while (step) {
        char operation = step->operation;
        unsigned long count = step->count;

        if (operation == LOOP_START_TOKEN) {
            step = (tape[index] ? step : step->loop)->next;
            continue;
        } else if (operation == LOOP_END_TOKEN) {
            step = step->loop;
            continue;
        }

        switch (operation) {
        case INC_TOKEN: tape[index] += count;
            break;
        case DEC_TOKEN: tape[index] -= count;
            break;
        case NEXT_TOKEN: index = next(index, count);
            break;
        case PREV_TOKEN: index = prev(index, count);
            break;
        case READ_TOKEN: read(tape, index, count);
            break;
        case WRITE_TOKEN: write(tape, index, count);
            break;
        default: break;
        }

        step = step->next;
    }
}

int run(char *source) {
    struct state *state = parse(source);

    if (state->errors != NULL) {
        while (state->errors != NULL) {
            struct step *step = NULL;
            state->errors = pop(state->errors, &step);
            log_loop_error(step);
        }

        return EXIT_FAILURE;
    }

    interpret(state->first);

    return EXIT_SUCCESS;
}