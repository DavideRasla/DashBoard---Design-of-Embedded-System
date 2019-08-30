#ifndef TYPES_H
#define TYPES_H

// Contains NULL declaration
#include <stddef.h>
#include <stdint.h>

// Defines a boolean type to be used in C
typedef enum { false, true } bool_t;

// Converts an expression to bool_t
#define BOOL(expr) (!!(expr))
#define U32T(expr) ((uint32_T)(expr))

typedef signed char int8_T;
typedef unsigned char uint8_T;
typedef short int16_T;
typedef unsigned short uint16_T;
typedef int int32_T;
typedef unsigned int uint32_T;


#endif
