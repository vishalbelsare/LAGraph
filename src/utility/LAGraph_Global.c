//------------------------------------------------------------------------------
// LAGraph_Global:  global variables for LAGraph
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause
// See additional acknowledgments in the LICENSE file,
// or contact permission@sei.cmu.edu for the full terms.

// Contributed by Timothy A. Davis, Texas A&M University

//------------------------------------------------------------------------------

#include "LG_internal.h"

//------------------------------------------------------------------------------
// memory management
//------------------------------------------------------------------------------

// These are modified by LAGraph_Init and LAGr_Init:
void * (* LAGraph_Malloc_function  ) (size_t)         = malloc ;
void * (* LAGraph_Calloc_function  ) (size_t, size_t) = calloc ;
void * (* LAGraph_Realloc_function ) (void *, size_t) = realloc ;
void   (* LAGraph_Free_function    ) (void *)         = free ;

//------------------------------------------------------------------------------
// threading control
//------------------------------------------------------------------------------

// These are initialized by LAGraph_Init and LAGr_Init, modified by
// LAGraph_SetNumThreads and accessed by LAGraph_GetNumThreads.
// They are not intendend to be directly access by the end user.

int LG_nthreads_outer ; // # of threads to use at the higher level of a nested
                        // parallel region in LAGraph.  Default: 1.

int LG_nthreads_inner ; // # of threads to use at the lower level of a nested
                        // parallel region, or to use inside GraphBLAS.
                        // Default: the value obtained by omp_get_max_threads
                        // if OpenMP is in use, or 1 otherwise.

