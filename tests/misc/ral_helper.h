#ifndef RAL_HELPER_H_
#define RAL_HELPER_H_

#include <stdio.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <lir/regalloc/ra.h>
#include "reg_helper.h"

static void colors_regalloc_dump_dot(map_t* colors) {
    fprintf(stdout, "graph colors {\n");
    fprintf(stdout, "  node [style=filled];\n");

    const char* palette[] = {
        "red",       "green",     "blue",   "yellow",
        "cyan",      "magenta",   "orange", "pink",
        "brown",     "grey",      "purple", "lime",
        "navy",      "teal",      "olive",  "maroon",
        "silver",    "gold",      "coral",  "violet",
        "indigo",    "turquoise", "beige",  "salmon",
        "chocolate", "plum",      "orchid", "tan",
        "crimson",   "khaki",     "azure",  "mint"
    };

    int palette_size = sizeof(palette) / sizeof(palette[0]);
    for (long i = 0; i < colors->capacity; i++) {
        if (colors->entries[i].used) {
            const char* fill = ((long)colors->entries[i].value >= 0 && (long)colors->entries[i].value < palette_size) ? palette[(long)colors->entries[i].value] : "white";
            fprintf(stdout, "  v%ld [fillcolor=%s (%s)];\n", colors->entries[i].key, fill, register_to_string((long)colors->entries[i].value));
        }
    }

    fprintf(stdout, "}\n");
}

#endif