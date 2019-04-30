#ifndef __PORTABLE_H__
#define __PORTABLE_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Colours */
/* Red */
#define  CR 0

/* Green */
#define  CG 1

/* Blue */
#define  CB 2

/* Yellow */
#define  CY 3

/* Black */
#define  CN 4

/* White */
#define  CW 5

/* Orientation */
#define ON 0
#define OE 1
#define OS 2
#define OW 3

/* Rotations */
#define RL 0
#define RR 1

typedef enum colour {RED = CR, GREEN = CG, BLUE = CB, YELLOW = CY, BLACK = CN, WHITE = CW} colour_t;
typedef enum orientation {NORTH = ON, SOUTH = OS, EAST = OE, WEST = OW} orientation_t;
typedef enum rotation {LEFT = RL, RIGHT = RR} rotation_t;

typedef struct {
    uint32_t x, y;
    orientation_t o;
} ant_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    colour_t **grid;
} square_grid_t;

#endif /* __PORTABLE_H__ */

