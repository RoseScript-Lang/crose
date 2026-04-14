#include "lib/librose.h"

void start_repl() {
    char line[256];
    printf("Welcome to RoseScript 0.5.0, Copyright 2026 RoseScript Devs\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "exit") == 0) break;
        if (line[0] == '\0') continue;

        interpret_line(line);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        start_repl();
        return 0;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        // Triggers the crash format if file doesn't exist
        rose_crash("File System", "Could not open source file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        interpret_line(line);
    }

    fclose(file);
    return 0;
}
