#include <ast/astgen/annot.h>

annotation_t* ANNOT_create_annotation(annotation_type_t t, string_t* data, long value) {
    annotation_t* annot = (annotation_t*)mm_malloc(sizeof(annotation_t));
    if (!annot) return NULL;
    annot->t = t;
    switch (t) {
        case ALIGN_ANNOTATION: annot->data.align = (int)value;           break;
        case SECTION_ANNOTATION: annot->data.section = data->copy(data); break;
        case ADDRESS_ANNOTATION: annot->data.address = value;            break;
        default: break;
    }

    return annot;
}

int ANNOT_read_annotations(sstack_t* annots, annotations_summary_t* summary) {
    annotation_t* annot;
    while (stack_pop(annots, (void**)&annot)) {
        switch (annot->t) {
            case SECTION_ANNOTATION: {
                if (summary->section) destroy_string(summary->section);
                summary->section = annot->data.section->copy(annot->data.section); 
                break;
            }
            case ALIGN_ANNOTATION: summary->align = annot->data.align; break;
            case NAKED_ANNOTATION: summary->is_naked = 1; break;
            case ADDRESS_ANNOTATION: summary->address = annot->data.address; break;
            default: break;
        }

        ANNOT_destroy_annotation(annot);
    }

    return 1;
}

int ANNOT_destroy_summary(annotations_summary_t* summray) {
    if (summray->section) destroy_string(summray->section);
    return 1;
}

int ANNOT_destroy_annotation(annotation_t* annot) {
    switch (annot->t) {
        case SECTION_ANNOTATION: destroy_string(annot->data.section); break;
        default: break;
    }

    mm_free(annot);
    return 1;
}
