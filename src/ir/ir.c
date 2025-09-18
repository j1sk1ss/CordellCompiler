#include <ir/ir.h>

ir_subject_t* IR_create_subject(int reg, int dref, int offset, char* name, int val, int size) {

}

ir_block_t* IR_create_block(ir_operation_t op, ir_subject_t* fa, ir_subject_t* sa, ir_subject_t* ta) {

}

int IR_insert_block(ir_block_t* block, ir_ctx_t* ctx) {

}

int IR_unload_blocks(ir_block_t* block) {

}

int IR_destroy_ctx(ir_ctx_t* ctx) {

}