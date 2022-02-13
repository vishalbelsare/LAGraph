//------------------------------------------------------------------------------
// test_sssp: test for LAGraph
//------------------------------------------------------------------------------

// LAGraph, (c) 2021 by The LAGraph Contributors, All Rights Reserved.
// SPDX-License-Identifier: BSD-2-Clause

//------------------------------------------------------------------------------

// Contributed by Jinhao Chen, Scott Kolodziej and Tim Davis, Texas A&M
// University

// Usage:
// test_sssp matrix.mtx sourcenodes.mtx delta
// test_sssp matrix.grb sourcenodes.mtx delta

#include "LAGraph_demo.h"
#include "LAGraphX.h"

// #define NTHREAD_LIST 1
// #define NTHREAD_LIST 2
// #define THREAD_LIST 0

#define NTHREAD_LIST 1
#define THREAD_LIST 0

#define LG_FREE_ALL                 \
{                                   \
    LAGraph_Delete (&G, NULL) ;     \
    GrB_free (&SourceNodes) ;       \
    GrB_free (&pathlen) ;           \
    GrB_free (&Delta) ;             \
}

int main (int argc, char **argv)
{
    char msg [LAGRAPH_MSG_LEN] ;

    LAGraph_Graph G = NULL ;
    GrB_Vector SourceNodes = NULL ;
    GrB_Vector pathlen = NULL ;
    GrB_Scalar Delta = NULL ;

    // start GraphBLAS and LAGraph
    bool burble = false ;
    demo_init (burble) ;
    LAGraph_Random_Init (msg) ;

    double tic [2] ;

    //--------------------------------------------------------------------------
    // determine # of threads to use
    //--------------------------------------------------------------------------

    int nt = NTHREAD_LIST ;
    int Nthreads [20] = { 0, THREAD_LIST } ;
    int nthreads_max ;
    LAGraph_TRY (LAGraph_GetNumThreads (&nthreads_max, NULL)) ;
    if (Nthreads [1] == 0)
    {
        // create thread list automatically
        Nthreads [1] = nthreads_max ;
        for (int t = 2 ; t <= nt ; t++)
        {
            Nthreads [t] = Nthreads [t-1] / 2 ;
            if (Nthreads [t] == 0) nt = t-1 ;
        }
    }
    printf ("threads to test: ") ;
    for (int t = 1 ; t <= nt ; t++)
    {
        int nthreads = Nthreads [t] ;
        if (nthreads > nthreads_max) continue ;
        printf (" %d", nthreads) ;
    }
    printf ("\n") ;

    //--------------------------------------------------------------------------
    // read in the graph
    //--------------------------------------------------------------------------

    char *matrix_name = (argc > 1) ? argv [1] : "stdin" ;
    printf ("\n---- Matrix: %s\n", matrix_name) ;
    GrB_Index nmatrices = 0 ;
    GrB_Matrix *Set = NULL ;
    char *collection ;
    LAGraph_TRY (LAGraph_SLoadSet (matrix_name, &Set, &nmatrices, &collection,
        msg)) ;

    // convert to FP32 and scale the matrix
    GrB_Index n, nvals ;
    GrB_TRY (GrB_Matrix_nrows (&n, Set [0])) ;
    GrB_Matrix T = NULL ;
    // T = abs (float (Set [0])) ;
    GrB_TRY (GrB_Matrix_new (&T, GrB_FP32, n, n)) ;
    GrB_TRY (GrB_apply (T, NULL, NULL, GrB_ABS_FP32, Set [0], NULL)) ;
    float emin, emax ;
    GrB_TRY (GrB_reduce (&emin, NULL, GrB_MIN_MONOID_FP32, T, NULL)) ;
    GrB_TRY (GrB_reduce (&emax, NULL, GrB_MAX_MONOID_FP32, T, NULL)) ;
    if (emin < 1e-3)
    {
        GrB_TRY (GrB_apply (T, NULL, NULL, GrB_PLUS_FP32, T, 1e-3, NULL)) ;
        emin = 1e-3 ;
    }
    float scale = 255. / emax ;
    GrB_TRY (GrB_apply (T, NULL, NULL, GrB_TIMES_FP32, T, scale, NULL)) ;

    LAGraph_TRY (LAGraph_New (&G, &T, LAGRAPH_ADJACENCY_DIRECTED, msg)) ;

    GrB_TRY (GrB_Matrix_nrows (&n, G->A)) ;
    GrB_TRY (GrB_Matrix_nvals (&nvals, G->A)) ;
    LAGraph_TRY (LAGraph_Property_Emin (G, msg)) ;

    //--------------------------------------------------------------------------
    // get delta
    //--------------------------------------------------------------------------

    float delta = 30 ;
    GrB_TRY (GrB_Scalar_new (&Delta, GrB_FP32)) ;
    GrB_TRY (GrB_Scalar_setElement (Delta, delta)) ;

    //--------------------------------------------------------------------------
    // get the source nodes
    //--------------------------------------------------------------------------

    GrB_Index nsource = 8 ;
    GrB_TRY (GrB_Vector_new (&SourceNodes, GrB_UINT64, nsource)) ;
    GrB_TRY (GrB_assign (SourceNodes, NULL, NULL, 1, GrB_ALL, nsource, NULL)) ;
    LAGraph_TRY (LAGraph_Random_Seed (SourceNodes, 1, msg)) ;
    // GxB_print (SourceNodes, 3) ;

    //--------------------------------------------------------------------------
    // warmup
    //--------------------------------------------------------------------------

    GrB_Index src ;
    GrB_TRY (GrB_Vector_extractElement (&src, SourceNodes, 0)) ;
    src = src % n ;
//  GxB_set (GxB_BURBLE, true) ;
    LAGraph_TRY (LAGraph_SingleSourceShortestPath (&pathlen,
        G, src, Delta, msg)) ;
    GxB_set (GxB_BURBLE, false) ;
    GrB_free (&pathlen) ;

    //--------------------------------------------------------------------------
    // begin tests
    //--------------------------------------------------------------------------

    int ntrials = (int) nsource ;
    for (int tt = 1 ; tt <= nt ; tt++)
    {
        int nthreads = Nthreads [tt] ;
        if (nthreads > nthreads_max) continue ;
        LAGraph_TRY (LAGraph_SetNumThreads (nthreads, msg)) ;
        double total_time = 0 ;

        for (int trial = 0 ; trial < ntrials ; trial++)
        {

            //------------------------------------------------------------------
            // get the source node for this trial
            //------------------------------------------------------------------

            // src = SourceNodes [trial] % n
            src = -1 ;
            GrB_TRY (GrB_Vector_extractElement (&src, SourceNodes, trial)) ;
            src = src % n ;
            double ttrial ;

            //------------------------------------------------------------------
            // sssp
            //------------------------------------------------------------------

            GrB_free (&pathlen) ;
            LAGraph_TRY (LAGraph_Tic (tic, msg)) ;
            LAGraph_TRY (LAGraph_SingleSourceShortestPath (&pathlen,
                G, src, Delta, msg)) ;
            LAGraph_TRY (LAGraph_Toc (&ttrial, tic, msg)) ;

            printf ("sssp15:  threads: %2d trial: %2d source %8ld "
                "time: %10.4f sec\n", nthreads, trial, src, ttrial) ;
            total_time += ttrial ;

#undef  LG_CHECK_RESULT
#define LG_CHECK_RESULT 1
#if LG_CHECK_RESULT
            // check result
            if (trial == 0)
            {
                // all trials can be checked, but this is slow so do just
                // for the first trial
                double tcheck ;
                LAGraph_Tic (tic, msg) ;
                int result = LG_check_sssp (pathlen, G, src, msg) ;
                if (result != GrB_SUCCESS)
                {
                    fprintf (stderr, "ERROR: %d %s\n", result, msg) ;
                }
                LAGraph_Toc (&tcheck, tic, msg) ;
                printf ("total check time: %g sec\n", tcheck) ;
            }
#endif
        }

        //----------------------------------------------------------------------
        // report results
        //----------------------------------------------------------------------

        printf ("\n") ;
        double e = (double) nvals ;
        total_time = total_time / ntrials ;
//      printf ("%2d: SSSP    time: %14.6f sec  rate: %8.2f (delta %g)\n",
//          nthreads, total_time, 1e-6 * e / total_time, delta);
        fprintf (stderr, "TIME: Avg: SSSP %3d: delta: %g time: %10.3f sec: %s\n",
             nthreads, delta, total_time, matrix_name) ;
         printf (        "TIME: Avg: SSSP %3d: delta: %g time: %10.3f sec: %s\n",
             nthreads, delta, total_time, matrix_name) ;
    }

    //--------------------------------------------------------------------------
    // free all workspace and finish
    //--------------------------------------------------------------------------

    LG_FREE_ALL ;
    LAGraph_Random_Finalize (msg) ;
    LAGraph_TRY (LAGraph_Finalize (msg)) ;
    return (GrB_SUCCESS) ;
}
