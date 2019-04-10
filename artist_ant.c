#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "ant_engine.h"
#include "artist_ant.h"

static uint32_t grid_width;
static uint32_t grid_height;
static square_grid_t grid;

static ant_t ant;
static void *palette;
static void *rules;

static uint32_t w, h, n;
static colour_t initial;

void show_version();

void show_help(char *);

static colour_t get_colour(char c);

int main(int argc, char **argv) {
    static struct option long_options[] = {
            {"grid",       1, 0, 'g'},
            {"palette",    1, 0, 'p'},
            {"rules",      1, 0, 'r'},
            {"iterations", 1, 0, 'n'},
            {"help",       0, 0, 'h'},
            {"version",    0, 0, 'v'},
            {0,            0, 0, 0}
    };
    static char *rule_spec, *grid_spec, *colour_spec;

    int opt, s, len = 0;
    char *dim_separator;
    int long_index = 0;

    ant_t *artist_ant;
    square_grid_t *square_grid;

    while ((opt = getopt_long(argc, argv, "g:p:r:n:hv", long_options, &long_index)) != -1) {

        switch (opt) {
            case 'g': /* grid */
                dim_separator = strchr(optarg, 'x');
                assert(dim_separator != NULL);

                s = (unsigned int) (dim_separator - optarg);
                len = strlen(optarg);

                grid_width = as_int(optarg, 0, s);
                grid_height = as_int(optarg, s + 1, len);

                break;
            case 'p': /* palette */
                colour_spec = (char *) malloc(strlen(optarg));
                memcpy(colour_spec, optarg, strlen(optarg));
                initial = get_colour(optarg[0]);

                break;

            case 'r': /* rules */
                rule_spec = (char *) malloc(strlen(optarg));
                memcpy(rule_spec, optarg, strlen(optarg));

                break;

            case 'n': /* times */
                n = atoi(optarg);

                if (n < 0) {
                    fprintf(stderr, "Must be non negative: %d", n);
                    show_help(argv[0]);
                    exit(1);
                }
                break;

            case 'h': /* help */
                show_help(argv[0]);
                exit(0);
                break;

            case 'v': /* version */
                show_version();
                exit(0);
                break;

            default:
                fprintf(stderr, "Unrecognized option %c\n", opt);
                show_help(argv[0]);
                exit(1);
        }
    }


    /*Check that we have a rule for each colouur */
    assert(strlen(rule_spec) == strlen(colour_spec));

    square_grid = make_grid(grid_width, grid_height, initial);
    artist_ant = make_ant(grid_width / 2, grid_height / 2);
    palette = make_palette(colour_spec);
    rules = make_rules(rule_spec, palette);
    paint(artist_ant, square_grid, palette, rules, n);

    grid_out();

    return 0;
}

void *
make_grid(uint32_t w, uint32_t h, colour_t c) {
    grid.width = w;
    grid.height = h;

    grid.grid = (uint32_t **) malloc(w * sizeof(uint32_t *));

    for (int i = 0; i < w; i++) {
        grid.grid[i] = (uint32_t *) malloc(h * sizeof(colour_t));
        for (int j = 0; j < h; j++) {
            grid.grid[i][j] = c;
        }
    }

    return &grid;
}

void *
make_ant(uint32_t xini, uint32_t yini) {
    ant.x = xini;
    ant.y = yini;
    ant.o = NORTH;

    return &ant;
}

void grid_out() {
    colour_t c;
    fprintf(stdout, "P3\n");
    fprintf(stdout, "%d %d\n", grid_width, grid_height);
    fprintf(stdout, "255\n");

    for (unsigned int i = 0; i < grid_width; i++) {
        for (unsigned int j = 0; j < grid_height; j++) {

            c = grid.grid[i][j];

            switch (c) {
                case RED:
                    printf("%d %-3d %-3d ", 255, 0, 0);
                    break;
                case GREEN:
                    printf("%-3d %d %-3d ", 0, 255, 0);
                    break;
                case BLUE:
                    printf("%-3d %-3d %d ", 0, 0, 255);
                    break;
                case WHITE:
                    printf("%d %d %d ", 255, 255, 255);
                    break;
                case YELLOW:
                    printf("%d %d %-3d ", 255, 255, 0);
                    break;
                case BLACK:
                    printf("%-3d %-3d %-3d ", 0, 0, 0);
                    break;
                default:
                    fprintf(stderr, "Invalid: %c\n", c);
                    exit(2);
            }
        }
        printf("\n");
    }
}

uint32_t
as_int(void *arg, uint32_t from, uint32_t to) {

    assert(from < to);

    const unsigned char *s = (unsigned char *) arg + from;
    unsigned char *t = (unsigned char *) (arg + to);

    uint32_t n = 0;

    while (s < t) {
        assert(*s >= '0' && *s <= '9');
        n = (*s - '0') + n * 10;
        s++;
    }

    return n;
}

void
show_help(char *p) {
    printf("  %s -g <dimensions> -p <colors> -r <rules> -t <n>\n", p);
    printf("  -g --grid: wxh\n");
    printf("  -p --palette: Combination of R|G|B|Y|N|W\n");
    printf("  -r --rules: Combination of L|R\n");
    printf("  -n --times: Iterations\n");
    printf("  -h --help: Print this message and exit\n");
    printf("  -v --verbose: Version number\n");
}

void
show_version() {
    printf("v0.0.0\n");
}

colour_t
get_colour(char c) {
    static char *index = "RGBYNW";

    char *p = strchr(index, c);

    assert (p != NULL);

    return (p - index);
}
