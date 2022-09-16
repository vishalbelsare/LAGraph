//------------------------------------------------------------------------------
// LAGraph_Malloc:  wrapper for malloc
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause
// See additional acknowledgments in the LICENSE file,
// or contact permission@sei.cmu.edu for the full terms.

// Contributed by Timothy A. Davis, Texas A&M University

//------------------------------------------------------------------------------

#include "LG_internal.h"

int LAGraph_Malloc
(
    // output:
    void **p,               // pointer to allocated block of memory
    // input:
    size_t nitems,          // number of items
    size_t size_of_item,    // size of each item
    char *msg
)
{
    // check inputs
    LG_CLEAR_MSG ;
    LG_ASSERT (p != NULL, GrB_NULL_POINTER) ;
    (*p) = NULL ;

    // make sure at least one item is allocated
    nitems = LAGRAPH_MAX (1, nitems) ;

    // make sure at least one byte is allocated
    size_of_item = LAGRAPH_MAX (1, size_of_item) ;

    // compute the size and check for integer overflow
    size_t size ;
    bool ok = LG_Multiply_size_t (&size, nitems, size_of_item) ;
    if (!ok || nitems > GrB_INDEX_MAX || size_of_item > GrB_INDEX_MAX)
    {
        // overflow
        return (GrB_OUT_OF_MEMORY) ;
    }

    // malloc the space
    (*p) = LAGraph_Malloc_function (size) ;
    return (((*p) == NULL) ? GrB_OUT_OF_MEMORY : GrB_SUCCESS) ;
}
