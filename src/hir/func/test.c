#include <hir/func.h>

static void _unload_test_info(func_info_t* info) {
    if (!info) return;
    destroy_string(info->name);
    destroy_string(info->virt);
    AST_unload(info->args);
    map_free(&info->template.generic);
    mm_free(info);
}

static func_info_t* _create_info_for_test_function(symbol_id_t id) {
    func_info_t* info = mm_malloc(sizeof(func_info_t));
    if (!info) return NULL;

    str_memset(info, 0, sizeof(func_info_t));
    info->id    = id;
    info->s_id  = NO_SYMBOL_ID;
    info->name  = create_string("test_entry");
    info->virt  = create_string(CONF_get_entry_name());
    info->args  = AST_create_node(NULL);
    info->flags = (func_info_flags_t) {
        .entry = 1, .global = 1, .used = 1,
        .vname = 1, .inln   = NEVER_INLINE
    };

    if (
        !info->name || 
        !info->virt || 
        !info->args || 
        !map_init(&info->template.generic, MAP_CMP)
    ) {
        _unload_test_info(info);
        return NULL;
    }

    return info;
}

static hir_block_t* _create_test_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta) {
    hir_block_t* block = HIR_create_block(op, fa, sa, ta);
    if (!block) {
        HIR_unload_subject(fa);
        HIR_unload_subject(sa);
        HIR_unload_subject(ta);
    }

    return block;
}

static int _append_test_call(hir_ctx_t* ctx, symbol_id_t id) {
    hir_subject_t *name = HIR_create_subject(HIR_FNAME, 0, NULL, 0), *args = HIR_SUBJ_LIST();
    if (!name || !args) {
        HIR_unload_subject(name);
        HIR_unload_subject(args);
        return 0;
    }

    name->storage.str.s_id = id;
    return HIR_append_block(_create_test_block(HIR_FCLL, NULL, name, args), ctx);
}

static void _unload_test_section(section_info_t* section) {
    if (!section) return;
    destroy_string(section->name);
    set_free(&section->vars);
    set_free(&section->func);
    set_free(&section->strs);
    set_free(&section->vtab);
    list_free(&section->sorted.func);
    mm_free(section);
}

static section_info_t* _create_test_section() {
    section_info_t* section = mm_malloc(sizeof(section_info_t));
    if (!section) return NULL;

    str_memset(section, 0, sizeof(section_info_t));
    section->name  = create_string(CONF_get_code_section());
    section->align = SMT_NULL;
    if (
        !section->name                        ||
        !set_init(&section->vars, SET_NO_CMP) ||
        !set_init(&section->func, SET_NO_CMP) ||
        !set_init(&section->strs, SET_NO_CMP) ||
        !set_init(&section->vtab, SET_NO_CMP)
    ) {
        _unload_test_section(section);
        return NULL;
    }

    return section;
}

static void _unload_test_func(cfg_func_t* fb) {
    if (!fb) return;
    foreach (cfg_block_t* bb, &fb->blocks) HIR_CFG_unload_block(bb);
    list_free(&fb->blocks);
    set_free(&fb->locals);
    set_free(&fb->leaders);
    mm_free(fb);
}

static void _replace_test_entry(hir_ctx_t* hctx, cfg_ctx_t* cctx, cfg_func_t* original, cfg_func_t* fb) {
    foreach (cfg_func_t* local, &cctx->funcs) {
        if (!set_has(&original->locals, local)) continue;
        hir_block_t *entry = local->hmap.entry, *exit = local->hmap.exit;
        entry->prev->next = exit->next;
        exit->next->prev  = entry->prev;
        entry->prev       = original->hmap.entry->prev;
        exit->next        = original->hmap.entry;
        if (entry->prev) entry->prev->next = entry;
        else             hctx->hot.h       = entry;
        original->hmap.entry->prev = exit;
    }

    hir_block_t *entry = original->hmap.entry, *exit = original->hmap.exit;
    fb->hmap.entry->prev = entry->prev;
    fb->hmap.exit->next  = exit->next;
    if (entry->prev) entry->prev->next = fb->hmap.entry;
    else             hctx->hot.h       = fb->hmap.entry;
    if (exit->next)  exit->next->prev  = fb->hmap.exit;
    else             hctx->hot.t       = fb->hmap.exit;
    entry->prev = exit->next = NULL;

    for (list_node_t* node = cctx->funcs.h; node; node = node->n) {
        if (node->data == original) {
            node->data = fb;
            break;
        }
    }

    _unload_test_func(original);
    HIR_unload_blocks(entry);
}

int HIR_FUNC_generate_test_function(hir_ctx_t* hctx, cfg_ctx_t* cctx, sym_table_t* smt) {
    if (!CONF_is_test_compilation()) return 1;
    if (!hctx || !cctx || !smt) return 0;

    cfg_func_t* original = NULL;
    foreach (cfg_func_t* candidate, &cctx->funcs) {
        if (!candidate->fentry) continue;
        if (original) return 0;
        original = candidate;
    }

    hir_ctx_t       generated   = { 0 };
    func_info_t*    info        = NULL;
    cfg_func_t*     fb          = NULL;
    cfg_block_t*    block       = NULL;
    section_info_t* section     = NULL;
    int             new_section = 0;
    if (original) {
        if (!map_get(&smt->f.functb, original->f_id, (void**)&info) || info->flags.local) goto _fail;
    }
    else {
        info = _create_info_for_test_function(smt->f.curr_id);
        if (!info) goto _fail;
    }

    hir_subject_t* name = HIR_create_subject(HIR_FNAME, 0, NULL, 0);
    if (!name) goto _fail;
    name->storage.str.s_id = info->id;
    if (
        !HIR_append_block(_create_test_block(HIR_FDCL, name, NULL, NULL), &generated) ||
        !HIR_append_block(HIR_create_block(HIR_MKSCOPE, NULL, NULL, NULL), &generated)
    ) goto _fail;

    foreach (cfg_func_t* test, &cctx->funcs) {
        func_info_t fi;
        if (!FNTB_get_info_id(test->f_id, &fi, &smt->f)) goto _fail;
        if (!fi.flags.testf || test == original || test->hmap.entry->unused) continue;
        if (
            fi.flags.vargs || fi.flags.self || fi.flags.generic ||
            (fi.args && fi.args->c && (!fi.args->c->t || fi.args->c->t->t_type != SCOPE_TOKEN))
        ) goto _fail;

        if (!_append_test_call(&generated, fi.id)) goto _fail;
    }

    hir_subject_t* status = HIR_SUBJ_CONST(0);
    if (!status) goto _fail;
    if (
        !HIR_append_block(_create_test_block(HIR_EXITOP, status, NULL, NULL), &generated) ||
        !HIR_append_block(HIR_create_block(HIR_ENDSCOPE, NULL, NULL, NULL), &generated)   ||
        !HIR_append_block(HIR_create_block(HIR_FEND, NULL, NULL, NULL), &generated)
    ) goto _fail;

    fb = HIR_func_create_funcblock(generated.hot.h);
    if (!fb || !set_is_init(&fb->locals) || !set_init(&fb->leaders, SET_NO_CMP)) goto _fail;
    fb->id        = original ? original->id : cctx->cid;
    fb->f_id      = info->id;
    fb->used      = fb->fentry = 1;
    fb->hmap.exit = generated.hot.t;

    block = HIR_CFG_create_cfg_block(generated.hot.h);
    if (!block) goto _fail;
    set_t* sets[] = {
        &block->visitors, &block->pred,     &block->curr_in,  &block->curr_out,
        &block->prev_in,  &block->prev_out, &block->def,      &block->use,
        &block->domf,     &block->dom,      &block->phi,      &block->copy_gen,
        &block->copy_kill
    };

    for (unsigned int i = 0; i < sizeof(sets) / sizeof(*sets); ++i) {
        if (!set_is_init(sets[i])) goto _fail;
    }

    block->id        = original ? cctx->cid : cctx->cid + 1;
    block->pfunc     = fb;
    block->hmap.exit = generated.hot.t;
    if (!list_add(&fb->blocks, block)) goto _fail;
    block = NULL;

    if (original) {
        if (!map_put(&cctx->fmap, info->id, fb)) goto _fail;
        _replace_test_entry(hctx, cctx, original, fb);
        info->flags.used = 1;
        info->flags.inln = NEVER_INLINE;
        cctx->cid++;
        return 1;
    }

    map_foreach (section_info_t* candidate, &smt->c.sectb) {
        if (candidate->name && candidate->name->requals(candidate->name, CONF_get_code_section())) {
            section = candidate;
            break;
        }
    }

    if (!section) {
        new_section = 1;
        section     = _create_test_section();
        if (
            !section ||
            map_get(&smt->c.sectb, (long)section->name->hash, NULL)
        ) goto _fail;
    }

    if (
        map_get(&smt->f.functb, info->id, NULL) || map_get(&cctx->fmap, info->id, NULL) ||
        !map_put(&smt->f.functb, info->id, info)
    )                                                                    goto _fail;
    if (!list_add(&cctx->funcs, fb))                                     goto _rollback_info;
    if (!map_put(&cctx->fmap, info->id, fb))                             goto _rollback_funcs;
    if (!set_add(&section->func, (void*)info->id))                       goto _rollback_fmap;
    if (!list_add(&section->sorted.func, (void*)info->id))               goto _rollback_section_func;
    if (new_section) {
        if (!map_put(&smt->c.sectb, (long)section->name->hash, section)) goto _rollback_section_list;
        if (!list_add(&smt->c.sorted.sectb, section)) {
            map_remove(&smt->c.sectb, (long)section->name->hash);
            goto _rollback_section_list;
        }
    }

    if (!hctx->hot.h) hctx->hot.h = generated.hot.h;
    else {
        hir_block_t* tail       = hctx->hot.h;
        while (tail->next) tail = tail->next;
        tail->next              = generated.hot.h;
        generated.hot.h->prev   = tail;
    }

    hctx->hot.t = generated.hot.t;
    smt->f.curr_id++;
    cctx->cid += 2;
    return 1;

_rollback_section_list: {}
    list_remove(&section->sorted.func, (void*)info->id);
_rollback_section_func: {}
    set_remove(&section->func, (void*)info->id);
_rollback_fmap: {}
    map_remove(&cctx->fmap, info->id);
_rollback_funcs: {}
    list_remove(&cctx->funcs, fb);
_rollback_info: {}
    map_remove(&smt->f.functb, info->id);
_fail: {}
    if (new_section) _unload_test_section(section);
    HIR_CFG_unload_block(block);
    _unload_test_func(fb);
    HIR_unload_blocks(generated.hot.h);
    if (!original) _unload_test_info(info);
    return 0;
}
