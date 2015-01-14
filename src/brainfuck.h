#define NEXT_TOKEN '>'
#define PREV_TOKEN '<'
#define INC_TOKEN '+'
#define DEC_TOKEN '-'
#define WRITE_TOKEN '.'
#define READ_TOKEN ','
#define LOOP_START_TOKEN '['
#define LOOP_END_TOKEN ']'

#define MAX_ARRAY_LENGTH 30000

struct step {
    unsigned long line;
    unsigned long position;
    char operation;
    unsigned long count;
    struct step *next;
    struct step *loop;
};

struct stack {
    struct step *value;
    struct stack *next;
};

void run(char *source);