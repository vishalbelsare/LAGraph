//------------------------------------------------------------------------------
// LAGraph_PMRinit: start GraphBLAS and LAGraph, and set malloc/etc functions
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause
// Contributed by Tim Davis, Texas A&M University.

//------------------------------------------------------------------------------

#include "LG_internal.h"

struct RMM_Handle ;

int LAGraph_PMRinit           // returns 0 if successful, -1 if failure
(
    // pmr allocate/deallocate memory management functions
    void * (* pmr_allocate_function   ) (size_t *),
    void   (* pmr_deallocate_function ) (void *p, size_t size),
    char *msg
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    // both are required
    LG_CLEAR_MSG ;
    LG_CHECK (pmr_allocate_function == NULL, -1, "allocate is NULL") ;
    LG_CHECK (pmr_deallocate_function   == NULL, -1, "deallocate is NULL") ;

    //--------------------------------------------------------------------------
    // create an RMM resource
    //--------------------------------------------------------------------------

    // size_t rmm_handle_size = 256;
    RMM_Handle *rmmH ;
    rmm_create_handle( &rmmH);
    size_t init_size = (1ULL<<10)*(1ULL<<8);
    size_t max_size =  (1ULL<<20)*(1ULL<<8);
    printf(" Hello before rmm_initialize %lu %lu\n", init_size, max_size);
    rmm_initialize( rmmH, rmm_managed, init_size, max_size );
    printf(" Hello after rmm_initialize\n");

    //--------------------------------------------------------------------------
    // start GraphBLAS
    //--------------------------------------------------------------------------

    GrB_TRY (GxB_pmr_init       // start up GraphBLAS for use with pmr
    (
        GrB_NONBLOCKING,
        // pmr allocate/deallocate memory management functions
        pmr_allocate_function,
        pmr_deallocate_function
    )) ;

    //--------------------------------------------------------------------------
    // save the memory management pointers in global LAGraph space
    //--------------------------------------------------------------------------

    // use ANSI C for LAGraph_Malloc and friends
    LAGraph_Malloc_function  = malloc ;
    LAGraph_Calloc_function  = calloc ;
    LAGraph_Realloc_function = realloc ;
    LAGraph_Free_function    = free ;

    LAGraph_Allocate_function = pmr_allocate_function ;
    LAGraph_Deallocate_function = pmr_deallocate_function ;
    LAGraph_RMM_Handle = (void *) rmmH ;

    LAGraph_Malloc_is_thread_safe = true ;

    return (0) ;
}

