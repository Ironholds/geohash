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

testthat::test_that("an NA or <1 precision produces an error", {

  testthat::expect_error(gh_encode(12.3, 1.3, -1))
  testthat::expect_error(gh_encode(12.3, 1.3, NA_integer_))

})


testthat::test_that(">10 precision produces a warning and 10-length entry", {

  testthat::expect_warning(gh_encode(12.3, 1.3, 11))
  testthat::expect_equal(nchar(gh_encode(12.3, 1.3, 11)), 10)

})
