% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/renderer.R
\name{boxes_latex}
\alias{boxes_latex}
\title{Creates the set of latex boxes}
\usage{
boxes_latex()
}
\value{
A character vector containing latex definitions for boxes
used by the latex renderer
}
\description{
This function returns the set of latex boxes definitions
that should be included in the document preamble. The 
latex renderer includes these definitions automatically when the 
document argument is TRUE, but not otherwise.
}
\seealso{
\code{\link{translator_latex}} translates text into markup that 
makes use of these boxes
}
