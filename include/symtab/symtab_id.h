#ifndef SYMTAB_ID_H_
#define SYMTAB_ID_H_
/* If there is any default value that must be used somewhere, you can this
   macro. Don't use the one below. It's only for update functions. */
#define SMT_NULL        -1
#define FIELD_NO_CHANGE -1 /* Selected field will be saved during upd op */
#define NO_SYMBOL_ID    -1 /* Default ID for all non-table data          */
typedef long symbol_id_t;  /* Object ID in symtables                     */
#endif
