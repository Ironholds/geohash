testthat::context("Test geohash bbox")

testthat::test_that("A simple lat/lng pair will correctly decoding", {
  raw_bbox <- geohash::gh_decode(geohash::gh_encode(lat = 39.949958,lng = 116.46343,precision = 8))
    
  res_bbox[[1]] <- list(lat = raw_bbox$lat - raw_bbox$lat_error,
                       lng = raw_bbox$lng - raw_bbox$lng_error)
  res_bbox[[2]] <- list(lat = raw_bbox$lat - raw_bbox$lat_error,
                       lng = raw_bbox$lng + raw_bbox$lng_error)
  res_bbox[[3]] <- list(lat = raw_bbox$lat + raw_bbox$lat_error,
                       lng = raw_bbox$lng + raw_bbox$lng_error)
  res_bbox[[4]] <- list(lat = raw_bbox$lat + raw_bbox$lat_error,
                       lng = raw_bbox$lng - raw_bbox$lng_error)

  testthat::expect_equal(res_bbox, gh_bbox(lat = 39.949958,lng = 116.46343,precision = 8))
 
})
