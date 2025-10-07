#ifndef X86_64_REGMAP_H_
#define X86_64_REGMAP_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/map.h>

typedef enum {
    REGISTER,
    MEMORY,
    NUMBER
} regval_type_t;

typedef struct {
    int reg;
} regmap_register_t;

typedef struct {
    long value;
} regmap_number_t;

typedef struct {
    long offset;
} regmap_memory_t;

typedef struct {
    regval_type_t         t;
    union {
        regmap_register_t reg;
        regmap_number_t   num;
        regmap_memory_t   mem;
    } storage;
} regmap_regval_t;

typedef struct {
    map_t regmap;
} regmap_t;

regmap_regval_t* REGMAP_create_value(regval_type_t type, int storage);
int REGMAP_update_storage(int reg, regmap_regval_t* value, regmap_t* regmap);
int REGMAP_get_storage(int reg, regmap_t* regmap, regmap_regval_t* value);

#endif