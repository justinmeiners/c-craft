
#ifndef ccraft_state_h
#define ccraft_state_h

#include "vec_math.h"

enum
{
    MODE_GAME = 0,
    MODE_INVENTORY,
};

typedef struct
{
    Vec2_t cursor;
    int mode;
} State_t;

#endif
