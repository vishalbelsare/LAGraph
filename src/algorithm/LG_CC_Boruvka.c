//------------------------------------------------------------------------------
// LG_CC_Boruvka.c:  connected components using GrB* methods only
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause
// See additional acknowledgments in the LICENSE file,
// or contact permission@sei.cmu.edu for the full terms.

// Contributed by Yongzhe Zhang (zyz915@gmail.com).
// Modified by Timothy A. Davis, Texas A&M University

//------------------------------------------------------------------------------

// This is an Advanced algorithm (G->is_symmetric_structure must be known),
// but it is not user-callable (see LAGr_ConnectedComponents instead).

// Code is based on Boruvka's minimum spanning forest algorithm.

// This method relies solely on GrB* methods in the V2.0 C API, but it much
// slower in general than LG_CC_FastSV6, which uses GxB pack/unpack methods
// for faster access to the contents of the matrices and vectors.

#include "LG_internal.h"

//------------------------------------------------------------------------------
// Reduce_assign
//------------------------------------------------------------------------------

// w[Px[i]] = min(w[Px[i]], s[i]) for i in [0..n-1].

static GrB_Info Reduce_assign
(
    GrB_Vector w,       // input/output vector of size n
    GrB_Vector s,       // input vector of size n
    GrB_Index *Px,      // Px: array of size n
    GrB_Index *mem,     // workspace of size 3*n
    GrB_Index n
)
{
    char *msg = NULL ;
    GrB_Index *ind  = mem ;
    GrB_Index *sval = ind + n ;
    GrB_Index *wval = sval + n ;
    GRB_TRY (GrB_Vector_extractTuples (ind, wval, &n, w)) ;
    GRB_TRY (GrB_Vector_extractTuples (ind, sval, &n, s)) ;
    for (GrB_Index j = 0 ; j < n ; j++)
    {
        if (sval [j] < wval [Px [j]])
        {
            wval [Px [j]] = sval [j] ;
        }
    }
    GRB_TRY (GrB_Vector_clear (w)) ;
    GRB_TRY (GrB_Vector_build (w, ind, wval, n, GrB_PLUS_UINT64)) ;
    LG_FREE_ALL ;
    return (GrB_SUCCESS) ;
}

//------------------------------------------------------------------------------
// select_func: IndexUnaryOp for pruning entries from S
//------------------------------------------------------------------------------

// Rather than using a global variable to access the Px array, it is passed to
// the select function as a uint64_t value that contains a pointer to Px.  It
// might be better to create a user-defined type for y, but this works fine.

void my_select_func (void *z, const void *x,
                 const GrB_Index i, const GrB_Index j, const void *y)
{
    GrB_Index *Px = (*(GrB_Index **) y) ;
    (*((bool *) z)) = (Px [i] != Px [j]) ;
}

//------------------------------------------------------------------------------
// LG_CC_Boruvka
//------------------------------------------------------------------------------

#undef  LG_FREE_ALL
#define LG_FREE_ALL                         \
{                                           \
    LG_FREE_WORK ;                          \
    GrB_free (&parent) ;                    \
}

#undef  LG_FREE_WORK
#define LG_FREE_WORK                        \
{                                           \
    LAGraph_Free ((void **) &I, NULL) ;     \
    LAGraph_Free ((void **) &Px, NULL) ;    \
    LAGraph_Free ((void **) &mem, NULL) ;   \
    GrB_free (&gp) ;                        \
    GrB_free (&mnp) ;                       \
    GrB_free (&ccmn) ;                      \
    GrB_free (&ramp) ;                      \
    GrB_free (&mask) ;                      \
    GrB_free (&select_op) ;                 \
}

int LG_CC_Boruvka
(
    // output:
    GrB_Vector *component,  // output: array of component identifiers
    // input:
    const LAGraph_Graph G,  // input graph, not modified
    char *msg
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    GrB_Index n, *I = NULL, *Px = NULL, *mem = NULL ;
    GrB_Vector parent = NULL, gp = NULL, mnp = NULL, ccmn = NULL, ramp = NULL,
        mask = NULL ;
    GrB_IndexUnaryOp select_op = NULL ;
    GrB_Matrix S = NULL ;

    LG_CLEAR_MSG ;
    LG_TRY (LAGraph_CheckGraph (G, msg)) ;
    LG_ASSERT (component != NULL, GrB_NULL_POINTER) ;

    LG_ASSERT_MSG ((G->kind == LAGraph_ADJACENCY_UNDIRECTED ||
       (G->kind == LAGraph_ADJACENCY_DIRECTED &&
        G->is_symmetric_structure == LAGraph_TRUE)),
        LAGRAPH_SYMMETRIC_STRUCTURE_REQUIRED,
        "G->A must be known to be symmetric") ;

    //--------------------------------------------------------------------------
    // initializations
    //--------------------------------------------------------------------------

    // S = structure of G->A
    LG_TRY (LAGraph_Matrix_Structure (&S, G->A, msg)) ;

    GRB_TRY (GrB_Matrix_nrows (&n, S)) ;
    GRB_TRY (GrB_Vector_new (&parent, GrB_UINT64, n)) ; // final result
    GRB_TRY (GrB_Vector_new (&gp, GrB_UINT64, n)) ;     // grandparents
    GRB_TRY (GrB_Vector_new (&mnp, GrB_UINT64, n)) ;    // min neighbor parent
    GRB_TRY (GrB_Vector_new (&ccmn, GrB_UINT64, n)) ;   // cc's min neighbor
    GRB_TRY (GrB_Vector_new (&mask, GrB_BOOL, n)) ;     // various uses

    LG_TRY (LAGraph_Malloc ((void **) &mem, 3*n, sizeof (GrB_Index), msg)) ;
    LG_TRY (LAGraph_Malloc ((void **) &Px, n, sizeof (GrB_Index), msg)) ;

    #if !LAGRAPH_SUITESPARSE
    // I is not needed for SuiteSparse and remains NULL
    LG_TRY (LAGraph_Malloc ((void **) &I, n, sizeof (GrB_Index), msg)) ;
    #endif

    // parent = 0:n-1, and copy to ramp
    GRB_TRY (GrB_assign (parent, NULL, NULL, 0, GrB_ALL, n, NULL)) ;
    GRB_TRY (GrB_apply  (parent, NULL, NULL, GrB_ROWINDEX_INT64, parent, 0,
        NULL)) ;
    GRB_TRY (GrB_Vector_dup (&ramp, parent)) ;

    // Px is a non-opaque copy of the parent GrB_Vector
    GRB_TRY (GrB_Vector_extractTuples (I, Px, &n, parent)) ;

    GRB_TRY (GrB_IndexUnaryOp_new (&select_op, my_select_func, GrB_BOOL,
        /* aij: ignored */ GrB_BOOL, /* y: pointer to Px */ GrB_UINT64)) ;

    GrB_Index nvals ;
    GRB_TRY (GrB_Matrix_nvals (&nvals, S)) ;

    //--------------------------------------------------------------------------
    // find the connected components
    //--------------------------------------------------------------------------

    while (nvals > 0)
    {

        //----------------------------------------------------------------------
        // mnp[u] = u's minimum neighbor's parent for all nodes u
        //----------------------------------------------------------------------

        // every vertex points to a root vertex at the begining
        GRB_TRY (GrB_assign (mnp, NULL, NULL, n, GrB_ALL, n, NULL)) ;
        GRB_TRY (GrB_mxv (mnp, NULL, GrB_MIN_UINT64,
                    GrB_MIN_SECOND_SEMIRING_UINT64, S, parent, NULL)) ;

        //----------------------------------------------------------------------
        // find the minimum neighbor
        //----------------------------------------------------------------------

        // ccmn[u] = connect component's minimum neighbor | if u is a root
        //         = n                                    | otherwise
        GRB_TRY (GrB_assign (ccmn, NULL, NULL, n, GrB_ALL, n, NULL)) ;
        GRB_TRY (Reduce_assign (ccmn, mnp, Px, mem, n)) ;

        //----------------------------------------------------------------------
        // parent[u] = ccmn[u] if ccmn[u] != n
        //----------------------------------------------------------------------

        // mask = (ccnm != n)
        GRB_TRY (GrB_apply (mask, NULL, NULL, GrB_NE_UINT64, ccmn, n, NULL)) ;
        // parent<mask> = ccmn
        GRB_TRY (GrB_assign (parent, mask, NULL, ccmn, GrB_ALL, n, NULL)) ;

        //----------------------------------------------------------------------
        // select new roots
        //----------------------------------------------------------------------

        // identify all pairs (u,v) where parent [u] == v and parent [v] == u
        // and then select the minimum of u, v as the new root;
        // if (parent [parent [i]] == i) parent [i] = min (parent [i], i)

        // compute grandparents: gp = parent (parent)
        GRB_TRY (GrB_Vector_extractTuples (I, Px, &n, parent)) ;
        GRB_TRY (GrB_extract (gp, NULL, NULL, parent, Px, n, NULL)) ;

        // mask = (gp == 0:n-1)
        GRB_TRY (GrB_eWiseMult (mask, NULL, NULL, GrB_EQ_UINT64, gp, ramp,
            NULL)) ;
        // parent<mask> = min (parent, ramp)
        GRB_TRY (GrB_assign (parent, mask, GrB_MIN_UINT64, ramp, GrB_ALL, n,
            NULL)) ;

        //----------------------------------------------------------------------
        // shortcutting: parent [i] = parent [parent [i]] until convergence
        //----------------------------------------------------------------------

        bool changing = true ;
        while (true)
        {
            // compute grandparents: gp = parent (parent)
            GRB_TRY (GrB_Vector_extractTuples (I, Px, &n, parent)) ;
            GRB_TRY (GrB_extract (gp, NULL, NULL, parent, Px, n, NULL)) ;

            // changing = or (parent != gp)
            GRB_TRY (GrB_eWiseMult (mask, NULL, NULL, GrB_NE_UINT64, parent, gp,
                NULL)) ;
            GRB_TRY (GrB_reduce (&changing, NULL, GrB_LOR_MONOID_BOOL, mask,
                NULL)) ;
            if (!changing) break ;

            // parent = gp
            GRB_TRY (GrB_assign (parent, NULL, NULL, gp, GrB_ALL, n, NULL)) ;
        }

        //----------------------------------------------------------------------
        // remove the edges inside each connected component
        //----------------------------------------------------------------------

        // This step is the costliest part of this algorithm when dealing with
        // large matrices.
        GRB_TRY (GrB_select (S, NULL, NULL, select_op, S, (uint64_t) Px, NULL));
        GRB_TRY (GrB_Matrix_nvals (&nvals, S)) ;
    }

    //--------------------------------------------------------------------------
    // free workspace and return result
    //--------------------------------------------------------------------------

    (*component) = parent ;
    LG_FREE_WORK ;
    return (GrB_SUCCESS) ;
}
