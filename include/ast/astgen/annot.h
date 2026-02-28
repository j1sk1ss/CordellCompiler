#ifndef ANNOT_H_
#define ANNOT_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>

typedef struct {
    string_t* section;
    int       align;
    long      address;
    char      is_naked : 1;
} annotations_summary_t;

typedef enum {
    ALIGN_ANNOTATION,   /* Set the align of a declaration            */
    SECTION_ANNOTATION, /* Put a declration or function to a section */
    NAKED_ANNOTATION,   /* Don't unpack START, FDECL                 */
    ADDRESS_ANNOTATION  /* Where place the object?                   */
} annotation_type_t;

typedef struct {
    annotation_type_t t;
    union {
        int           align;   /* ALIGN_ANNOTATION   */
        string_t*     section; /* SECTION_ANNOTATION */
        long          address; /* ADDRESS_ANNOTATION */
    } data;
} annotation_t;

int ANNOT_read_annotations(sstack_t* annots, annotations_summary_t* summary);
int ANNOT_destroy_summary(annotations_summary_t* summray);
annotation_t* ANNOT_create_annotation(annotation_type_t t, string_t* data, long value);
int ANNOT_destroy_annotation(annotation_t* annot);

#endif
