#ifndef ANNOT_H_
#define ANNOT_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>

#define ENTRY_ANNOTATION_COMMAND "entry"
#define ALIGN_ANNOTATION_COMMAND "align"
#define NAKED_ANNOTATION_COMMAND "naked"
#define SECTN_ANNOTATION_COMMAND "section"
#define ADDRS_ANNOTATION_COMMAND "address"
#define NOFAL_ANNOTATION_COMMAND "no_fall"
#define STRGH_ANNOTATION_COMMAND "straight"
#define COUNT_ANNOTATION_COMMAND "counter"

typedef struct {
    string_t* section;
    int       align;
    long      address;
    long      counter;
    char      is_naked    : 1;
    char      is_entry    : 1;
    char      is_nofall   : 1;
    char      is_straight : 1;
} annotations_summary_t;

typedef enum {
    UNKNOWN_ANNOTATION,
    ALIGN_ANNOTATION,    /* Set the align of a declaration            */
    SECTION_ANNOTATION,  /* Put a declration or function to a section */
    NAKED_ANNOTATION,    /* Don't unpack START, FDECL                 */
    ADDRESS_ANNOTATION,  /* Where place the object?                   */
    ENTRY_ANNOTATION,    /* Is this an entry function?                */
    NOFALL_ANNOTATION,   /* switch with a break as a default command  */
    STRAIGHT_ANNOTATION, /* switch based on if-elseif-else            */
    COUNTER_ANNOTATION,  /* hidden counter-break instructure          */
} annotation_type_t;

typedef struct {
    annotation_type_t t;
    union {
        int           align;   /* ALIGN_ANNOTATION   */
        string_t*     section; /* SECTION_ANNOTATION */
        long          address; /* ADDRESS_ANNOTATION */
        long          counter; /* COUNTER_ANNOTATION */
    } data;
} annotation_t;

int ANNOT_read_annotations(sstack_t* annots, annotations_summary_t* summary);
int ANNOT_destroy_summary(annotations_summary_t* summray);
annotation_t* ANNOT_create_annotation(annotation_type_t t, string_t* data, long value);
int ANNOT_destroy_annotation(annotation_t* annot);

#endif
