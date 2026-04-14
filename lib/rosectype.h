#ifndef ROSECTYPE_H
#define ROSECTYPE_H

#include <ctype.h>
#include <stdbool.h>

// RoseScript C-Type Definitions
typedef double  rose_val;   // Rose 'val' maps to C double
typedef char* rose_var;   // Rose 'var' maps to C char pointer
typedef char    rose_str;   // Rose 'string' maps to C char

typedef enum {
    TYPE_VAL,
    TYPE_VAR,
    TYPE_UNKNOWN
} RoseType;

// Helper to determine type of a value string
RoseType get_rose_type(char* input) {
    if (input[0] == '"') return TYPE_VAR;

    // Check if it's a number
    char* p = input;
    if (*p == '-') p++;
    while (*p) {
        if (!isdigit(*p) && *p != '.') return TYPE_UNKNOWN;
        p++;
    }
    return TYPE_VAL;
}

#endif
