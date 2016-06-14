testthat::context("Test geohash encoding")

testthat::test_that("A simple lat/lng pair will correctly encode", {

  hash <- gh_encode(42.60498046875, -5.60302734375, 5)
  testthat::expect_equal(length(hash), 1)
  testthat::expect_equal(hash[1], "ezs42")

})

test_that("Incorrect lengths are appropriately handled when encoding", {

  testthat::expect_error(gh_encode(c(42.60498046875,12.3), -5.60302734375, 5),
                         regexp = "The lats and lngs vectors must be the same size")

})

testthat::test_that("NAs are appropriately handled when encoding", {

  hash <- gh_encode(c(12.3, NA), c(1.34,-5.60302734375), 5)

  testthat::expect_true(is.na(hash[2]))

})
