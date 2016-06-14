testthat::context("Test geohash neighbour generation")

testthat::test_that("All neighbours can be found for a simple hash", {

  result <- gh_neighbours("ezs42")
  testthat::expect_that(result$north[1], testthat::equals("ezs48"))
  testthat::expect_that(result$northeast[1], testthat::equals("ezs49"))
  testthat::expect_that(result$east[1], testthat::equals("ezs43"))
  testthat::expect_that(result$southeast[1], testthat::equals("ezs41"))
  testthat::expect_that(result$south[1], testthat::equals("ezs40"))
  testthat::expect_that(result$southwest[1], testthat::equals("ezefp"))
  testthat::expect_that(result$west[1], testthat::equals("ezefr"))
  testthat::expect_that(result$northwest[1], testthat::equals("ezefx"))
})

test_that("NAs are appropriately handled with neighbouring", {

  result <- gh_neighbours(c("ezs42", NA))
  expect_true(is.na(result$north[2]))

})

test_that("Individual neighbour extraction works", {
  hash <- "ezs42"
  testthat::expect_that(north(hash), testthat::equals("ezs48"))
  testthat::expect_that(northeast(hash), testthat::equals("ezs49"))
  testthat::expect_that(east(hash), testthat::equals("ezs43"))
  testthat::expect_that(southeast(hash), testthat::equals("ezs41"))
  testthat::expect_that(south(hash), testthat::equals("ezs40"))
  testthat::expect_that(southwest(hash), testthat::equals("ezefp"))
  testthat::expect_that(west(hash), testthat::equals("ezefr"))
  testthat::expect_that(northwest(hash), testthat::equals("ezefx"))
})
