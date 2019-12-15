#' @importFrom grDevices rgb postscript dev.off pdf
#' @importFrom tools vignetteEngine
#' @useDynLib highlight, .registration = TRUE
#' @importFrom grDevices col2rgb colors
#' @importFrom utils Rtangle RweaveChunkPrefix RweaveEvalWithOpt
#'   RweaveLatexFinish RweaveLatexOptions RweaveLatexSetup RweaveTryStop
#'   Sweave SweaveHooks getParseData tail
NULL

NAMESPACE <- environment()

.onLoad <- function(libname, pkgname){
	options( detective = simple_detective )

	vignetteEngine("highlight", weave = Hweave, tangle = Htangle,
        pattern = "[.][hHrRsS]nw$",
        package = "highlight")
}

