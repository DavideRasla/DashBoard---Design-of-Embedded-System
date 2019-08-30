#ifndef TYPES_H
#define TYPES_H

// Contains NULL declaration
#include <stddef.h>
#include <stdint.h>

// Defines a boolean type to be used in C
typedef enum { false, true } bool_t;

// Converts an expression to bool_t
#define BOOL(expr) (!!(expr))

typedef unsigned char uint8_T;



#endif
