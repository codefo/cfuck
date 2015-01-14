#include <stdio.h>
#include <stdlib.h>

#include "brainfuck.h"

#define PROGRAM_NAME "cfuck"
#define VERSION "0.2.1"
#define YEAR "2015"
#define AUTHOR "Gleb Pospelov <codefo@codefo.com>"

#define ERROR_FILE_NOT_OPENED "Input file not opened"

int run_file(char *path) {
    char *buffer = NULL;
    FILE *file = NULL;
    size_t length;

    file = fopen(path, "rb");

    if (file == NULL) {
        fputs(ERROR_FILE_NOT_OPENED, stderr);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    length = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(length);

    if (buffer) {
        fread(buffer, 1, length, file);
    }

    fclose(file);

    return run(buffer);
}

void run_repl() {
    printf("%s %s %s\n", PROGRAM_NAME, VERSION, YEAR);
    printf("%s\n\n", AUTHOR);

    unsigned long index = 1;
    char *line = NULL;
    size_t length = 0;

    while(index)
    {
        printf("%lu> ", index);

        if (feof(stdin) != 0) break;

        getline(&line, &length, stdin);
        run(line);

        free(line);
        line = NULL;

        index++;
    }
}

int main(int argc, char *argv[]) {

    if (argc > 1) {
        return run_file(argv[1]);
    } else {
        run_repl();
    }

    return EXIT_SUCCESS;
}

