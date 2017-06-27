#include <R.h>
#include <Rinternals.h>
#include <stdlib.h>
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP geohash_gh_decode(SEXP);
extern SEXP geohash_gh_encode(SEXP, SEXP, SEXP);
extern SEXP geohash_gh_neighbour(SEXP, SEXP);
extern SEXP geohash_gh_neighbours(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"geohash_gh_decode",     (DL_FUNC) &geohash_gh_decode,     1},
    {"geohash_gh_encode",     (DL_FUNC) &geohash_gh_encode,     3},
    {"geohash_gh_neighbour",  (DL_FUNC) &geohash_gh_neighbour,  2},
    {"geohash_gh_neighbours", (DL_FUNC) &geohash_gh_neighbours, 1},
    {NULL, NULL, 0}
};

void R_init_geohash(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
