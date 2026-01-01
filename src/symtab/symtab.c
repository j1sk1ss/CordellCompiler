#include <symtab/symtab.h>

sym_table_t* SMT_create() {
    sym_table_t* smt = (sym_table_t*)mm_malloc(sizeof(sym_table_t));
    if (!smt) return NULL;
    str_memset(smt, 0, sizeof(sym_table_t));
    return smt;
}

int SMT_init(sym_table_t* smt) {
    smt->v.curr_id = 0;
    smt->s.curr_id = 0;
    smt->f.curr_id = 0;
    map_init(&smt->v.vartb, MAP_NO_CMP);
    map_init(&smt->s.strtb, MAP_NO_CMP);
    map_init(&smt->f.functb, MAP_NO_CMP);
    map_init(&smt->a.arrtb, MAP_NO_CMP);
    map_init(&smt->m.allias, MAP_NO_CMP);
    return 1;
}

int SMT_compress(sym_table_t* smt) {
    map_compress(&smt->v.vartb);
    map_compress(&smt->s.strtb);
    map_compress(&smt->f.functb);
    return 1;
}

int SMT_unload(sym_table_t* smt) {
    VRTB_unload(&smt->v);
    ARTB_unload(&smt->a);
    FNTB_unload(&smt->f);
    STTB_unload(&smt->s);
    ALLIAS_unload(&smt->m);
    mm_free(smt);
    return 1;
}
