#include <symtab/arrtb.h>

int ARTB_get_info(symbol_id_t id, array_info_t* info, arrtab_ctx_t* ctx) {
    print_log("ARTB_get_info(id=%li)", id);
    array_info_t* ai;
    if (map_get(&ctx->arrtb, id, (void**)&ai)) {
        if (info) str_memcpy(info, ai, sizeof(array_info_t));
        return 1;
    }

    return 0;
}

int ARTB_add_elems(symbol_id_t id, array_elem_info_t elem, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_elems(id=%li, elem_type=%i)", id, elem.t);
    array_info_t* ai;
    if (map_get(&ctx->arrtb, id, (void**)&ai)) {
        array_elem_info_t* copy = (array_elem_info_t*)mm_malloc(sizeof(array_elem_info_t));
        if (!copy) return 0;
        str_memcpy(copy, &elem, sizeof(array_elem_info_t));
        if (list_add(&ai->elems, copy)) return 1;
        mm_free(copy);
    }

    return 0;
} 

static array_info_t* _create_info_array_entry(symbol_id_t id, long size, int vla, token_type_t el_type, basic_object_info_t* flags) {
    array_info_t* entry = (array_info_t*)mm_malloc(sizeof(array_info_t));
    if (!entry) return NULL;
    str_memset(entry, 0, sizeof(array_info_t));

    entry->v_id                  = id;
    entry->vla                   = vla;
    entry->size                  = size;
    entry->elements_info.el_type = el_type;
    
    str_memcpy(&entry->elements_info.el_flags, flags, sizeof(basic_object_info_t));
    list_init(&entry->elems);
    return entry;
}

symbol_id_t ARTB_add_copy(symbol_id_t nid, array_info_t* src, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_copy(id=%li, src=%li)", nid, src->v_id);
    array_info_t* nnd = _create_info_array_entry(nid, src->size, src->vla, src->elements_info.el_type, &src->elements_info.el_flags);
    if (!nnd) return NO_SYMBOL_ID;
    foreach (array_elem_info_t* elem, &src->elems) {
        array_elem_info_t* copy = (array_elem_info_t*)mm_malloc(sizeof(array_elem_info_t));
        if (!copy) {
            list_free_force(&nnd->elems);
            mm_free(nnd);
            return NO_SYMBOL_ID;
        }
        
        str_memcpy(copy, elem, sizeof(array_elem_info_t));
        if (!list_add(&nnd->elems, copy)) {
            mm_free(copy);
            list_free_force(&nnd->elems);
            mm_free(nnd);
            return NO_SYMBOL_ID;
        }
    }

    map_put(&ctx->arrtb, nnd->v_id, nnd);
    return nnd->v_id;
}

symbol_id_t ARTB_add_info(symbol_id_t id, long size, int vla, token_type_t el_type, basic_object_info_t* flags, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_info(vid=%i, size=%i, vla=%i, el_type=%i)", id, size, vla, el_type);
    array_info_t* nnd = _create_info_array_entry(id, size, vla, el_type, flags);
    if (!nnd) return NO_SYMBOL_ID;
    map_put(&ctx->arrtb, id, nnd);
    return nnd->v_id;
}

int ARTB_unload(arrtab_ctx_t* ctx) {
    map_foreach (array_info_t* ai, &ctx->arrtb) {
        list_free_force(&ai->elems);
    }
    
    return map_free_force(&ctx->arrtb);
}
