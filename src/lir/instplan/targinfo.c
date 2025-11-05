#include <lir/instplan/targinfo.h>

int TRGINF_load(char* path, target_info_t* s) {
    map_init(&s->info);
    print_debug("TRGINF_load(path=%s)", path);
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fread(s, sizeof(s->name) + sizeof(int), 1, f);
    for (int i = 0; i < s->op_count; i++) {
        op_info_t* info = (op_info_t*)mm_malloc(sizeof(op_info_t));
        fread(info, sizeof(op_info_t), 1, f);
        map_put(&s->info, info->op, info);

        print_debug(
            "op=%i, comm=%i, issue=%i, latency=%i, rm=%i, sf=%i, th=%i, rf=%i, wm=%i", 
            info->op, info->commutative, info->issue_cost, 
            info->latency, info->reads_memory, info->sets_flags, 
            info->throughput, info->uses_flags, info->writes_memory
        );
    }

    fclose(f);
    return 1;
}

int TRGINF_unload(target_info_t* s) {
    return map_free_force(&s->info);
}

#ifdef TARGINFO_BUILD
#include <string.h>
#include <stdlib.h>
static op_info_t* _create_op_info(lir_operation_t op, int rm, int wm, int sf, int rf, int lat, float th, int cost, int cmt) {
    op_info_t* opinf = (op_info_t*)malloc(sizeof(op_info_t));
    if (!opinf) return NULL;
    opinf->op            = op;
    opinf->reads_memory  = rm;
    opinf->writes_memory = wm;
    opinf->sets_flags    = sf;
    opinf->uses_flags    = rf;
    opinf->latency       = lat;
    opinf->throughput    = th;
    opinf->issue_cost    = cost;
    opinf->commutative   = cmt;
    return opinf;
}

static int _save_target_info(const char* filename, target_info_t* trginfo) {
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    fwrite(trginfo->name, sizeof(char), 32, f);
    fwrite(&trginfo->op_count, sizeof(int), 1, f);
    if (trginfo->op_count > 0 && trginfo->ops != NULL) {
        fwrite(trginfo->ops, sizeof(op_info_t), trginfo->op_count, f);
    }

    fclose(f);
    return 1;
}

#define ADD_OPINF(s, i) map_put(s, (i)->op, (void*)i)
int main(int argc, char* argv[]) {
    mm_init();

    map_t ops;
    map_init(&ops);

    if (!strcmp(argv[1], "Ivy_Bridge")) {
        ADD_OPINF(&ops, _create_op_info(LIR_FCLL, 1, 1, 0, 0, 100, 0.1f, 10, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_ECLL, 1, 1, 0, 0, 100, 0.1f, 10, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_SYSC, 1, 1, 0, 0, 200, 0.05f, 15, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_FRET, 1, 0, 0, 0, 5, 0.5f, 3, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_JMP, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_JL, 0, 0, 0, 1, 1, 2.0f, 1, 0)); 
        ADD_OPINF(&ops, _create_op_info(LIR_JG, 0, 0, 0, 1, 1, 2.0f, 1, 0)); 
        ADD_OPINF(&ops, _create_op_info(LIR_JLE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_JGE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_JE, 0, 0, 0, 1, 1, 2.0f, 1, 0)); 
        ADD_OPINF(&ops, _create_op_info(LIR_JNE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_JB, 0, 0, 0, 1, 1, 2.0f, 1, 0)); 
        ADD_OPINF(&ops, _create_op_info(LIR_JA, 0, 0, 0, 1, 1, 2.0f, 1, 0)); 
        ADD_OPINF(&ops, _create_op_info(LIR_JBE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_JAE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_TST, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_CMP, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_XCHG, 1, 1, 0, 0, 10, 0.5f, 2, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_iMOV, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMOVb, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMOVw, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMOVd, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMOVq, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMVZX, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMVSX, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_GDREF, 1, 0, 0, 0, 4, 1.0f, 2, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_LDREF, 0, 1, 0, 0, 3, 1.0f, 2, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_REF, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_PUSH, 0, 1, 0, 0, 2, 1.5f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_POP, 1, 0, 0, 0, 2, 1.5f, 1, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_iADD, 0, 0, 1, 0, 1, 3.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_iSUB, 0, 0, 1, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMUL, 0, 0, 1, 0, 3, 1.0f, 2, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_iDIV, 0, 0, 1, 0, 20, 0.2f, 5, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iMOD, 0, 0, 1, 0, 25, 0.15f, 6, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_iLRG, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iLGE, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iLWR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iLRE, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iCMP, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_iNMP, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        
        ADD_OPINF(&ops, _create_op_info(LIR_iAND, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_iOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_NOT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_bAND, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_bOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_bXOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_bSHL, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_bSHR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_bSAR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iBLFT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_iBRHT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        
        ADD_OPINF(&ops, _create_op_info(LIR_fMOV, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_fMVf, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_fADD, 0, 0, 0, 0, 3, 1.0f, 2, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_fSUB, 0, 0, 0, 0, 3, 1.0f, 2, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_fMUL, 0, 0, 0, 0, 4, 0.5f, 2, 1));
        ADD_OPINF(&ops, _create_op_info(LIR_fDIV, 0, 0, 0, 0, 10, 0.2f, 4, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_fCMP, 0, 0, 1, 0, 3, 1.0f, 2, 1));
        
        ADD_OPINF(&ops, _create_op_info(LIR_CDQ, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_ALLCH, 1, 1, 0, 0, 50, 0.3f, 5, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_DEALLH, 1, 0, 0, 0, 30, 0.4f, 4, 0));
        ADD_OPINF(&ops, _create_op_info(LIR_EXITOP, 0, 0, 0, 0, 1, 1.0f, 1, 0));
    }

    target_info_t trginfo = {
        .op_count = ops.size,
        .ops = calloc(ops.size, sizeof(op_info_t))
    };

    strcpy(trginfo.name, argv[1]);

    int index = 0;
    map_iter_t it;
    map_iter_init(&ops, &it);
    op_info_t* i;
    while (map_iter_next(&it, (void**)&i)) {
        memcpy(&trginfo.ops[index++], i, sizeof(op_info_t));
    }

    _save_target_info(argv[2], &trginfo);
    free(trginfo.ops);

    map_free_force(&ops);
    #undef ADD_OPINF
    return 0;
}
#endif