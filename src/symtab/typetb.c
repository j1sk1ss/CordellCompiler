#include <symtab/typetb.h>

/*
Create a new entry for a type.
Params:
    - `lv` - Linked Variable.

Returns NULL or a new entry.
*/
static type_entry_info_t* _create_entry(symbol_id_t lv) {
    type_entry_info_t* entry = (type_entry_info_t*)mm_malloc(sizeof(type_entry_info_t));
    if (!entry) return NULL;
    entry->vid = lv;
    return entry;
}

static type_info_t* _create_type_info(string_t* name) {
    type_info_t* info = (type_info_t*)mm_malloc(sizeof(type_info_t));
    if (!info) return NULL;
    list_init(&info->entries);
    info->name = name->copy(name);
    return info;
}

symbol_id_t TPTB_add_info(string_t* name, typetab_ctx_t* ctx) {
    if (TPTB_get_info(name, NULL, ctx)) return -1;
    type_info_t* info = _create_type_info(name);
    if (!info) return -1;
    info->id = ctx->curr_id++;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

int TPTB_info_add_entry(symbol_id_t tid, symbol_id_t vid, typetab_ctx_t* ctx) {
    type_info_t* info;
    if (!map_get(&ctx->typetb, tid, (void**)&info)) {
        return 0;
    }

    type_entry_info_t* entry = _create_entry(vid);
    if (!entry) return 0;

    list_add(&info->entries, entry);
    return 1;
}

int TPTB_get_info_id(symbol_id_t tid, type_info_t* info, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (map_get(&ctx->typetb, tid, (void**)&ti)) {
        if (info) str_memcpy(info, ti, sizeof(type_info_t));
        return 1;
    }

    return 0;
}

int TPTB_get_info(string_t* name, type_info_t* info, typetab_ctx_t* ctx) {
    map_foreach (type_info_t* ti, &ctx->typetb) {
        if (name->equals(name, ti->name)) {
            if (info) str_memcpy(info, ti, sizeof(type_info_t));
            return 1;
        }
    }

    return 0;
}

static int _unload_info(type_info_t* info) {
    list_free_force(&info->entries);
    destroy_string(info->name);
    mm_free(info);
    return 1;
}

int TPTB_unload(typetab_ctx_t* ctx) {
    return map_free_force_op(&ctx->typetb, (int (*)(void*))_unload_info);
}
