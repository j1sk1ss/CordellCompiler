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

int ARTB_add_elems(symbol_id_t id, long elem, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_elems(id=%li, elem=%li)", id, elem);
    array_info_t* ai;
    if (map_get(&ctx->arrtb, id, (void**)&ai)) {
        array_elem_info_t* eli = (array_elem_info_t*)mm_malloc(sizeof(array_elem_info_t));
        eli->value = elem;
        return list_add(&ai->elems, eli);
    }

    return 0;
} 

static array_info_t* _create_info_array_entry(symbol_id_t id, long size, int heap, token_type_t el_type, token_flags_t* flags) {
    array_info_t* entry = (array_info_t*)mm_malloc(sizeof(array_info_t));
    if (!entry) return NULL;
    str_memset(entry, 0, sizeof(array_info_t));
    entry->v_id = id;
    entry->heap = heap;
    entry->size = size;
    entry->elements_info.el_type = el_type;
    str_memcpy(&entry->elements_info.el_flags, flags, sizeof(token_flags_t));
    list_init(&entry->elems);
    return entry;
}

symbol_id_t ARTB_add_copy(symbol_id_t nid, array_info_t* src, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_copy(id=%li, src=%li)", nid, src->v_id);
    array_info_t* nnd = _create_info_array_entry(nid, src->size, src->heap, src->elements_info.el_type, &src->elements_info.el_flags);
    if (!nnd) return 0;
    map_put(&ctx->arrtb, nnd->v_id, nnd);
    return nnd->v_id;
}

int ARTB_update_info(symbol_id_t id, long size, int heap, token_type_t el_type, token_flags_t* flags, arrtab_ctx_t* ctx) {
    print_log("ARTB_update_info(vid=%i, size=%i, heap=%i, el_type=%i)", id, size, heap, el_type);
    array_info_t* ai;
    if (map_get(&ctx->arrtb, id, (void**)&ai)) {
        if (el_type >= 0) ai->elements_info.el_type = el_type;
        if (flags)        str_memcpy(&ai->elements_info.el_flags, flags, sizeof(token_flags_t));
        if (size >= 0)    ai->size = size;
        if (heap >= 0)    ai->heap = heap;
        return 1;
    }

    return 0;
}

symbol_id_t ARTB_add_info(symbol_id_t id, long size, int heap, token_type_t el_type, token_flags_t* flags, arrtab_ctx_t* ctx) {
    print_log("ARTB_add_info(vid=%i, size=%i, heap=%i, el_type=%i)", id, size, heap, el_type);
    array_info_t* nnd = _create_info_array_entry(id, size, heap, el_type, flags);
    if (!nnd) return 0;
    map_put(&ctx->arrtb, id, nnd);
    return nnd->v_id;
}

int ARTB_unload(arrtab_ctx_t* ctx) {
    map_foreach (array_info_t* ai, &ctx->arrtb) {
        list_free_force(&ai->elems);
    }
    
    return map_free_force(&ctx->arrtb);
}
