#ifdef TARGINFO_BUILD
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lir/instplan/targinfo.h>

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

#define ADD_OPINF(s, i)                        \
    do {                                       \
        op_info_t* __tmp = i;                  \
        map_put(s, (__tmp)->op, (void*)__tmp); \
    } while (0)

#define IVY_BRIDGE       "Ivy_Bridge"
#define KABY_LAKE        "Kaby_Lake"
#define COFFEE_LAKE      "Coffee_Lake"

#define I7_7700K         "i7_7700k"
#define I7_7700K_DASH    "i7-7700k"
#define I7_8550U         "i7_8550u"
#define I7_8550U_DASH    "i7-8550u"
#define I5_9400F         "i5_9400f"
#define I5_9400F_DASH    "i5-9400f"

static void _add_ivy_bridge_ops(map_t* ops) {
        ADD_OPINF(ops, _create_op_info(LIR_FCLL, 1, 1, 0, 0, 100, 0.1f, 10, 0));
        ADD_OPINF(ops, _create_op_info(LIR_ECLL, 1, 1, 0, 0, 100, 0.1f, 10, 0));
        ADD_OPINF(ops, _create_op_info(LIR_SYSC, 1, 1, 0, 0, 200, 0.05f, 15, 0));
        ADD_OPINF(ops, _create_op_info(LIR_FRET, 1, 0, 0, 0, 5, 0.5f, 3, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JMP, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JL, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JG, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JLE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JGE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JNE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JB, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JA, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JBE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JAE, 0, 0, 0, 1, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_TST, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_CMP, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_XCHG, 1, 1, 0, 0, 10, 0.5f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMOV, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_aMOV, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LOADFARG, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LOADFRET, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_STARGLD, 0, 0, 0, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_GDREF, 1, 0, 0, 0, 4, 1.0f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LDREF, 0, 1, 0, 0, 3, 1.0f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_REF_GDREF, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_REF, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_PUSH, 0, 1, 0, 0, 2, 1.5f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_POP, 1, 0, 0, 0, 2, 1.5f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iADD, 0, 0, 1, 0, 1, 3.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iSUB, 0, 0, 1, 0, 1, 3.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMUL, 0, 0, 1, 0, 3, 1.0f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iDIV, 0, 0, 1, 0, 20, 0.2f, 5, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMOD, 0, 0, 1, 0, 25, 0.15f, 6, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLRG, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLGE, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLWR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLRE, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iCMP, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iNMP, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iAND, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_NEG, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_NOT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bAND, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bXOR, 0, 0, 1, 0, 1, 2.0f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bSHL, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bSHR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bSAR, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iBLFT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iBRHT, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fMOV, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fMVf, 0, 0, 0, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fADD, 0, 0, 0, 0, 3, 1.0f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_fSUB, 0, 0, 0, 0, 3, 1.0f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fMUL, 0, 0, 0, 0, 4, 0.5f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_fDIV, 0, 0, 0, 0, 10, 0.2f, 4, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fCMP, 0, 0, 1, 0, 3, 1.0f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_CDQ, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_CQO, 0, 0, 1, 0, 1, 2.0f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_EXITOP, 0, 0, 0, 0, 1, 1.0f, 1, 0));
}

static void _add_skl_client_like_ops(map_t* ops) {
        /* Calls/syscalls are ABI/runtime-level costs, not single-instruction ALU timings. */
        ADD_OPINF(ops, _create_op_info(LIR_FCLL, 1, 1, 0, 0, 100, 0.10f, 10, 0));
        ADD_OPINF(ops, _create_op_info(LIR_ECLL, 1, 1, 0, 0, 100, 0.10f, 10, 0));
        ADD_OPINF(ops, _create_op_info(LIR_SYSC, 1, 1, 0, 0, 200, 0.05f, 15, 0));
        ADD_OPINF(ops, _create_op_info(LIR_FRET, 1, 0, 0, 0, 5, 0.50f, 3, 0));

        /* Branches: modeled as cheap when predicted. Intel notes predicted jcc latency is effectively zero. */
        ADD_OPINF(ops, _create_op_info(LIR_JMP, 0, 0, 0, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JL,  0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JG,  0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JLE, 0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JGE, 0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JE,  0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JNE, 0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JB,  0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JA,  0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JBE, 0, 0, 0, 1, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_JAE, 0, 0, 0, 1, 1, 2.00f, 1, 0));

        /* Integer ALU, reg-reg form: Intel table gives 1-cycle latency and 0.25-cycle reciprocal throughput. */
        ADD_OPINF(ops, _create_op_info(LIR_TST,  0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_CMP,  0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMOV, 0, 0, 0, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_aMOV, 0, 0, 0, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LOADFARG, 0, 0, 0, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LOADFRET, 0, 0, 0, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_STARGLD,  0, 0, 0, 0, 1, 4.00f, 1, 0));

        /* Memory/reference operations: modeled for L1-hit common case. */
        ADD_OPINF(ops, _create_op_info(LIR_XCHG, 1, 1, 0, 0, 22, 0.045f, 10, 0));
        ADD_OPINF(ops, _create_op_info(LIR_GDREF, 1, 0, 0, 0, 4, 2.00f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_LDREF, 0, 1, 0, 0, 1, 1.00f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_REF_GDREF, 0, 0, 0, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_REF,       0, 0, 0, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_PUSH, 0, 1, 0, 0, 1, 1.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_POP,  1, 0, 0, 0, 4, 1.00f, 1, 0));

        ADD_OPINF(ops, _create_op_info(LIR_iADD, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iSUB, 0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMUL, 0, 0, 1, 0, 3, 1.00f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iDIV, 0, 0, 1, 0, 25, 0.040f, 7, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iMOD, 0, 0, 1, 0, 25, 0.040f, 7, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLRG, 0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLGE, 0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLWR, 0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iLRE, 0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iCMP, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iNMP, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iAND, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_iOR,  0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_NEG,  0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_NOT,  0, 0, 1, 0, 1, 4.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bAND, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bOR,  0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bXOR, 0, 0, 1, 0, 1, 4.00f, 1, 1));
        ADD_OPINF(ops, _create_op_info(LIR_bSHL, 0, 0, 1, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bSHR, 0, 0, 1, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_bSAR, 0, 0, 1, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iBLFT, 0, 0, 1, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_iBRHT, 0, 0, 1, 0, 1, 2.00f, 1, 0));

        /* Scalar SSE-like floating-point heuristic. */
        ADD_OPINF(ops, _create_op_info(LIR_fMOV, 0, 0, 0, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fMVf, 0, 0, 0, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fADD, 0, 0, 0, 0, 4, 2.00f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_fSUB, 0, 0, 0, 0, 4, 2.00f, 2, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fMUL, 0, 0, 0, 0, 4, 2.00f, 2, 1));
        ADD_OPINF(ops, _create_op_info(LIR_fDIV, 0, 0, 0, 0, 14, 0.25f, 5, 0));
        ADD_OPINF(ops, _create_op_info(LIR_fCMP, 0, 0, 1, 0, 2, 1.00f, 2, 1));

        ADD_OPINF(ops, _create_op_info(LIR_CDQ, 0, 0, 1, 0, 1, 1.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_CQO, 0, 0, 1, 0, 1, 2.00f, 1, 0));
        ADD_OPINF(ops, _create_op_info(LIR_EXITOP, 0, 0, 0, 0, 1, 1.00f, 1, 0));
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stdout, "Argmunet count must be equals to 3!\n");
        return EXIT_FAILURE;
    }

    mm_init();

    map_t ops;
    map_init(&ops, MAP_NO_CMP);

#pragma region [Archs]
#define STREQ(a) (!strcmp((a), argv[1]))
    if (STREQ(IVY_BRIDGE)) _add_ivy_bridge_ops(&ops);
    else if (
        STREQ(KABY_LAKE) ||
        STREQ(I7_7700K) || STREQ(I7_7700K_DASH) ||
        STREQ(I7_8550U) || STREQ(I7_8550U_DASH)
    ) _add_skl_client_like_ops(&ops);
    else if (
        STREQ(COFFEE_LAKE) ||
        STREQ(I5_9400F) || STREQ(I5_9400F_DASH)
    ) _add_skl_client_like_ops(&ops);
    else {
        fprintf(stdout, "Unsupported target architecture: %s\n", argv[1]);
        map_free_force(&ops);
        return EXIT_FAILURE;
    }
#undef STREQ
#pragma endregion

    target_info_t trginfo = {
        .op_count = ops.size,
        .ops = calloc(ops.size, sizeof(op_info_t))
    };

    if (!trginfo.ops) {
        fprintf(stdout, "Failed to allocate target info operations!\n");
        map_free_force(&ops);
        return EXIT_FAILURE;
    }

    strcpy(trginfo.name, argv[1]);

    int index = 0;
    map_foreach (op_info_t* i, &ops) {
        memcpy(&trginfo.ops[index++], i, sizeof(op_info_t));
    }

    int ok = _save_target_info(argv[2], &trginfo);
    free(trginfo.ops);

    map_free_force(&ops);
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
