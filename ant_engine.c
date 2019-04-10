#include "ant_engine.h"
#include "artist_ant.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void paint(void *ant, void *grid, void *palette, void *rules, uint32_t n) {
    ant_t *a = (ant_t *) ant;
    square_grid_t *g = (square_grid_t *) grid;
    enum colour *cs = (enum colour *) palette;
    enum rotation *rs = (enum rotation *) rules;
    int j = 1;
    int num_colors = 3;
//    int num_rows = sizeof(g->grid) / sizeof(g->grid[0]);
    int num_rows = g->height;
    int num_cols = g->width;
    for (int i = 0; i < n; i++) {
        // Rotar
        enum colour c_actual = g->grid[a->x][a->y];
        enum rotation r_actual = rs[c_actual];
        if (r_actual == RR) {
            a->o++;
            if (a->o == 4) a->o = 0;
        } else {
            a->o--;
            if (a->o == -1) a->o = 3;
        }

        // Pintar
        if (j == num_colors) j = 0;
        enum colour c_pintar = cs[j];
        j++;
        g->grid[a->x][a->y] = c_pintar;

        // Avanzar
        if (a->o == ON) {
            a->x--;
            if (a->x == -1) a->x = num_rows - 1;
        } else if (a->o == OE) {
            a->y++;
            if (a->y == num_cols) a->y = 0;
        } else if (a->o == OS) {
            a->x++;
            if (a->x == num_rows) a->x = 0;
        } else if (a->o == OW) {
            a->y--;
            if (a->y == -1) a->y = num_cols - 1;
        }
    }
}

void *make_rules(char *rs, enum colour *cs) {
    int n;
    if (strlen(rs) == 1) n = 1;
    else n = (strlen(rs) + 1) / 2;
    enum rotation *rules = malloc(sizeof(enum rotation) * 6);
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (rs[j] == 'L') rules[cs[i]] = RL;
        else if (rs[j] == 'R') rules[cs[i]] = RR;
        j += 2;
    }
    return rules;
}

void *make_palette(unsigned char *colours) {
    int n;
    if (strlen(colours) == 1) n = 1;
    else n = (strlen(colours) + 1) / 2;
    enum colour *colors = malloc(sizeof(enum colour) * n);
    int j = 0;
    static char *index = "RGBYNW";
    for (int i = 0; i < n; i++) {
        char *p = strchr(index, colours[j]);
        colors[i] = p - index;
        j += 2;
    }
    return colors;
}
