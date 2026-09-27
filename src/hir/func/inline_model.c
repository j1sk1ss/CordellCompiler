#include <hir/inline_model.h>

static double inline_model_score(const double features[INLINE_MODEL_FEATURE_COUNT])
{
    unsigned int tree;
    unsigned int split_offset = 0;
    unsigned int leaf_offset = 0;
    double result = 0.0;

    for (tree = 0; tree < INLINE_MODEL_TREE_COUNT; ++tree) {
        unsigned int depth;
        unsigned int index = 0;
        for (depth = 0; depth < inline_model_tree_depth[tree]; ++depth) {
            unsigned int split_index = split_offset + depth;
            unsigned int feature_index = inline_model_split_feature[split_index];
            if (features[feature_index] > inline_model_split_border[split_index]) {
                index |= 1u << depth;
            }
        }
        result += inline_model_leaf_values[leaf_offset + index];
        split_offset += inline_model_tree_depth[tree];
        leaf_offset += 1u << inline_model_tree_depth[tree];
    }
    return result;
}

int inline_model_should_inline(const double features[INLINE_MODEL_FEATURE_COUNT])
{
    return inline_model_score(features) >= INLINE_MODEL_RAW_THRESHOLD;
}