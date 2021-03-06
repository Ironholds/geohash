% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{gh_decode}
\alias{gh_decode}
\title{Decode Geohashes}
\usage{
gh_decode(hashes)
}
\arguments{
\item{hashes}{a character vector of geohashes.}
}
\value{
a data.frame of four columns; "lat", "lng", "lat_error" and "lng_error"
}
\description{
\code{gh_decode} takes geohashes and turns them back into
latitude/longitude pairs, with an associated margin of error for each value.
}
\examples{
# A simple example:
gh_encode(lat = 42.60498046875, lng = -5.60302734375, precision = 5)
#[1] "ezs42"

gh_decode("ezs42")
# lat      lng      lat_error  lng_error
# 42.60498 -5.603027 0.02197266 0.02197266
}
\seealso{
\code{\link{gh_encode}} for generating geohashes, and
\code{\link{gh_neighbours}} for identifying the neighbouring hash boxes
to a geohash.
}
