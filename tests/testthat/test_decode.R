testthat::context("Test geohash decoding")

testthat::test_that("A simple lat/lng pair will correctly decoding", {

  result <- gh_decode("ezs42")
  testthat::expect_equal(result$gh[1L], "ezs42")
  testthat::expect_lt(result$lat[1], 43)
  testthat::expect_gt(result$lat[1], 42)
  testthat::expect_lt(result$lng[1], -4)
  testthat::expect_gt(result$lng[1], -6)
})

testthat::test_that("NAs are appropriately handled when decoding", {

  result <- gh_decode(c("ezs42", NA))
  testthat::expect_true(is.na(result$lat[2]))

})
