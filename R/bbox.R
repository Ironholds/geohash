
#'@title Get Latitude and Longitude of Bounding Box
#'@description \code{\link{gh_bbox}} generates bounding box list of a
#'given precision from vectors of latitude and longitude values.
#'
#'@param lats a numeric vector of latitude values.
#'
#'@param lngs a numeric vector of longitude values. Must be the same length
#'as \code{lat}.
#'
#'@param precision an integer representing the precision the hashes should have.
#'This should be between 1 and 10; if the precision requested is greater than 10, it will
#'use 10 - if less than 1, it will error.
#'
#'@return a double list of bouding box, according to lat lng precision input.
#'
#'@examples
#'# A simple example:
#'gh_bbox(lat = 39.949958, lng = 116.46343, precision = 8)
#'#[[1]]
#'#[[1]]$lat
#'#[1] 39.94989
#'#[[1]]$lng
#'#[1] 116.4633
#'#[[2]]
#'#[[2]]$lat
#'#[1] 39.94989
#'#[[2]]$lng
#'#[1] 116.4637
#'#[[3]]
#'#[[3]]$lat
#'#[1] 39.95007
#'#[[3]]$lng
#'#[1] 116.4637
#'#[[4]]
#'#[[4]]$lat
#'#[1] 39.95007
#'#[[4]]$lng
#'#[1] 116.4633
#'
#'@export
gh_bbox <- function(lat = 39.949958, lng = 116.46343, precision = 8){
   raw_bbox <- geohash::gh_decode(geohash::gh_encode(lat,lng,precision))  
   
   res_bbox = list()
   
   res_bbox[[1]] <- list(lat = raw_bbox$lat - raw_bbox$lat_error,
                         lng = raw_bbox$lng - raw_bbox$lng_error)
   res_bbox[[2]] <- list(lat = raw_bbox$lat - raw_bbox$lat_error,
                         lng = raw_bbox$lng + raw_bbox$lng_error)
   res_bbox[[3]] <- list(lat = raw_bbox$lat + raw_bbox$lat_error,
                         lng = raw_bbox$lng + raw_bbox$lng_error)
   res_bbox[[4]] <- list(lat = raw_bbox$lat + raw_bbox$lat_error,
                         lng = raw_bbox$lng - raw_bbox$lng_error)
   return(res_bbox)
}
