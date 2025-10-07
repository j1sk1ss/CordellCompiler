#include <lir/x86_64_gnu_nasm/opt/x86_64_regmap.h>

regmap_regval_t* REGMAP_create_value(regval_type_t type, int storage) {
    regmap_regval_t* regval = (regmap_regval_t*)mm_malloc(sizeof(regmap_regval_t));
    if (!regval) return NULL;
    regval->t = type;

    switch (type) {
        case REGISTER: regval->storage.reg.reg = storage;    break;
        case MEMORY:   regval->storage.mem.offset = storage; break;
        case NUMBER:   regval->storage.num.value  = storage; break;
    }

    return regval;
}

int REGMAP_update_storage(int reg, regmap_regval_t* value, regmap_t* regmap) {
    if (!value) return 0;
    regmap_regval_t* rv;
    if (map_get(&regmap->regmap, reg, (void**)&rv)) mm_free(rv);
    return map_put(&regmap->regmap, reg, value);
}

int REGMAP_get_storage(int reg, regmap_t* regmap, regmap_regval_t* value) {
    regmap_regval_t* rv;
    if (map_get(&regmap->regmap, reg, (void**)&rv)) {
        if (value) str_memcpy(value, rv, sizeof(regmap_regval_t));
        return 1;
    }

    return 0;
}
