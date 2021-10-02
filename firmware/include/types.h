#pragma once

typedef unsigned char uint8_t;
_Static_assert(sizeof(uint8_t) == 1, "Wrong size of type!");

typedef char int8_t;
_Static_assert(sizeof(int8_t) == 1, "Wrong size of type!");

typedef unsigned short uint16_t;
_Static_assert(sizeof(uint16_t) == 2, "Wrong size of type!");

typedef short int16_t;
_Static_assert(sizeof(int16_t) == 2, "Wrong size of type!");

typedef unsigned long uint32_t;
_Static_assert(sizeof(uint32_t) == 4, "Wrong size of type!");

typedef long int32_t;
_Static_assert(sizeof(int32_t) == 4, "Wrong size of type!");

typedef int8_t bool;
#define true 1
#define false 0
