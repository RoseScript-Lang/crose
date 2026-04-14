#ifndef LIBROSE_H
#define LIBROSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rosectype.h"

// ANSI Color Definitions
#define ANSI_LIGHT_RED    "\033[1;31m"
#define ANSI_DARK_YELLOW "\033[0;33m"
#define ANSI_LIGHT_BLUE   "\033[1;34m"
#define ANSI_DARK_GREY    "\033[1;30m"
#define ANSI_RESET        "\033[0m"

typedef struct {
    char name[64];
    rose_val num_val;
    char str_val[128];
    int is_num;
} Variable;

Variable memory[200];
int var_count = 0;

char current_scope[128] = "Global";
char last_raw_line[256] = "";
int skip_block = 0; // Global flag to handle 'if' skipping

// The requested crash format with ANSI colors
void rose_crash(const char* type, const char* detail) {
    printf("\n%scrash:%s(%s%s - %s%s)\n",
           ANSI_LIGHT_RED, ANSI_RESET,
           ANSI_DARK_YELLOW, type, detail, ANSI_RESET);

    printf("%sstack traceback:%s\n", ANSI_LIGHT_BLUE, ANSI_RESET);
    printf("%s    in %s\n", ANSI_DARK_GREY, current_scope);
    printf("    >> %s%s\n\n", last_raw_line, ANSI_RESET);

    exit(1);
}

// Internal helper to get variable or literal value
double get_value(char* token) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(memory[i].name, token) == 0) return memory[i].num_val;
    }
    return atof(token);
}

double evaluate_math(char* expr) {
    char term1[64], term2[64], op[3];
    // Support for comparisons and basic math
    if (sscanf(expr, "%s %s %s", term1, op, term2) == 3) {
        double a = get_value(term1);
        double b = get_value(term2);

        if (strcmp(op, "+") == 0) return a + b;
        if (strcmp(op, "-") == 0) return a - b;
        if (strcmp(op, "*") == 0) return a * b;
        if (strcmp(op, "/") == 0) return b != 0 ? a / b : 0;
        if (strcmp(op, "==") == 0) return a == b;
        if (strcmp(op, "!=") == 0) return a != b;
        if (strcmp(op, ">") == 0) return a > b;
        if (strcmp(op, "<") == 0) return a < b;
    }
    return get_value(expr);
}

void interpret_line(char* line);

void load_module(char* module_path) {
    char path[256] = {0};
    strcpy(path, module_path);
    for(int i = 0; path[i]; i++) if(path[i] == '.') path[i] = '/';
    strcat(path, ".rose");

    FILE* mod_file = fopen(path, "r");
    if (!mod_file) {
        rose_crash("Not such module", module_path);
        return;
    }

    char buf[256];
    fpos_t loop_start;
    int in_while = 0;
    char while_cond[128] = "";

    while (fgets(buf, sizeof(buf), mod_file)) {
        // Basic While Loop Logic
        if (strncmp(buf, "while", 5) == 0) {
            sscanf(buf, "while %[^ {]", while_cond);
            fgetpos(mod_file, &loop_start);
            in_while = 1;
        }

        interpret_line(buf);

        if (in_while && strstr(buf, "}")) {
            if (evaluate_math(while_cond)) {
                fsetpos(mod_file, &loop_start);
            } else {
                in_while = 0;
            }
        }
    }
    fclose(mod_file);
}

void set_variable(char* name, char* value, int is_num) {
    for(int i = 0; i < var_count; i++) {
        if(strcmp(memory[i].name, name) == 0) {
            if(is_num) memory[i].num_val = evaluate_math(value);
            else strcpy(memory[i].str_val, value);
            return;
        }
    }
    strcpy(memory[var_count].name, name);
    memory[var_count].is_num = is_num;
    if(is_num) memory[var_count].num_val = evaluate_math(value);
    else strcpy(memory[var_count].str_val, value);
    var_count++;
}

void interpret_line(char* line) {
    // Clean whitespace
    while(*line == ' ' || *line == '\t') line++;
    if (*line == '\n' || *line == '/' || *line == '\0') return;

    strcpy(last_raw_line, line);

    // Block handling for 'if'
    if (skip_block) {
        if (strchr(line, '}')) skip_block = 0;
        return;
    }

    // Logic: If Statement
    if (strncmp(line, "if", 2) == 0) {
        char cond[128];
        if (sscanf(line, "if %[^ {]", cond) == 1) {
            if (!evaluate_math(cond)) skip_block = 1;
        }
        return;
    }

    // Existing Scope/Class Logic
    if (strstr(line, "class ")) {
        char cname[64];
        if(sscanf(line, "%*s class %s", cname) == 1) strcpy(current_scope, cname);
        return;
    }
    if (strstr(line, "void ")) {
        char fname[64];
        if(sscanf(line, "void %[^ (]", fname) == 1) {
            strcat(current_scope, ".");
            strcat(current_scope, fname);
        }
        return;
    }

    // Ignore brackets during standard execution
    if (strchr(line, '{') || strchr(line, '}')) return;

    if (strncmp(line, "module", 6) == 0) {
        char mname[64];
        if (sscanf(line, "module %[^; ]", mname) != 1) rose_crash("Expected", "module name");
        load_module(mname);
    }
    else if (strncmp(line, "printl", 6) == 0) {
        char content[128];
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (!start || !end) rose_crash("Expected", "parentheses ()");

        int len = end - start - 1;
        strncpy(content, start + 1, len);
        content[len] = '\0';

        if (content[0] == '"') {
            content[strlen(content)-1] = '\0';
            printf("%s\n", content + 1);
        } else {
            int found = 0;
            for(int i = 0; i < var_count; i++) {
                if(strcmp(memory[i].name, content) == 0) {
                    if(memory[i].is_num) printf("%g\n", (double)memory[i].num_val);
                    else printf("%s\n", memory[i].str_val);
                    found = 1; break;
                }
            }
            if(!found) {
                if (strchr(content, '.')) rose_crash("Not such member", content);
                else printf("%s\n", content);
            }
        }
    }
    else if (strncmp(line, "string", 6) == 0) {
        char name[64], prompt[128], input_buf[128];
        if (!strchr(line, '=')) rose_crash("Expected", "=");
        char *q_start = strchr(line, '"'), *q_end = strrchr(line, '"');
        if (q_start && q_end) {
            sscanf(line, "string %s =", name);
            strncpy(prompt, q_start + 1, q_end - q_start - 1);
            prompt[q_end - q_start - 1] = '\0';
            printf("%s ", prompt);
            fgets(input_buf, sizeof(input_buf), stdin);
            input_buf[strcspn(input_buf, "\n")] = 0;
            set_variable(name, input_buf, 0);
        } else rose_crash("Expected", "prompt in quotes");
    }
    else if (strncmp(line, "val", 3) == 0) {
        char name[64], expr[128];
        char *eq = strchr(line, '=');
        if (!eq) rose_crash("Expected", "=");
        sscanf(line, "val %s", name);
        strcpy(expr, eq + 1);
        char *semi = strchr(expr, ';'); if (semi) *semi = '\0';
        set_variable(name, expr, 1);
    }
    else if (strncmp(line, "var", 3) == 0) {
        char name[64], val[128];
        char *eq = strchr(line, '=');
        if (!eq) rose_crash("Expected", "=");
        char *q_start = strchr(line, '"'), *q_end = strrchr(line, '"');
        if (q_start && q_end) {
            sscanf(line, "var %s =", name);
            strncpy(val, q_start + 1, q_end - q_start - 1);
            val[q_end - q_start - 1] = '\0';
            set_variable(name, val, 0);
        } else rose_crash("Expected", "quotes");
    }
}
#endif
