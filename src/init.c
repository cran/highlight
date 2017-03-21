#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP HighlightMain(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP get_highlighted_text(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP top_level(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"HighlightMain",        (DL_FUNC) &HighlightMain,         7},
    {"get_highlighted_text", (DL_FUNC) &get_highlighted_text, 10},
    {"top_level",            (DL_FUNC) &top_level,             1},
    {NULL, NULL, 0}
};

void R_init_highlight(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
