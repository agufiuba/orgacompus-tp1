#ifndef __ANT_ENGINE_H__
#define __ANT_ENGINE_H__

#include "ant_constants.h"
#include "paint.h"

void* make_rules(char *spec, enum colour*);

void* make_palette(unsigned char *colours);

void* make_grid(uint32_t w, uint32_t h, colour_t c);

void* make_ant(uint32_t xini, uint32_t yini);

void grid_out();

uint32_t as_int(void *arg, uint32_t from, uint32_t to);

#endif /* __ANT_ENGINE_H__ */
