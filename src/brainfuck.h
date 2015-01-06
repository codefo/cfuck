#define NEXT_TOKEN '>'
#define PREV_TOKEN '<'
#define INC_TOKEN '+'
#define DEC_TOKEN '-'
#define WRITE_TOKEN '.'
#define READ_TOKEN ','
#define LOOP_START_TOKEN '['
#define LOOP_END_TOKEN ']'

#define MAX_ARRAY_LENGTH 30000

struct instruction {
    char operation;
    unsigned long count;
    unsigned long line;
    unsigned long position;
    struct instruction *next;
    struct instruction *loop;
};

struct node {
    struct instruction *value;
    struct node *next;
};

void run(char *source);