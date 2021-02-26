library(pryr)

test_that("pickle of base types works", {
  numericVector <- runif(100)
  numericVectorAddress <- address(numericVector)

  integerVector <- 1L:100L
  integerVectorAddress <- address(integerVector)

  characterVector <- sample(LETTERS, 100, replace = TRUE)
  characterVectorAddress <- address(characterVector)

  logicalVector <- sample(c(TRUE, FALSE), 100, replace = TRUE)
  logicalVectorAddress <- address(logicalVector)

  expect_vector((serializedPickle <- pickle(numericVector)))
  unserializedPickle <- unserialize(serializedPickle)
  expect_named(unserializedPickle[["objects"]], numericVectorAddress)
  expect_identical(unserializedPickle[["objects"]][[numericVectorAddress]], numericVector)

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(numericVector, unpickledValues)

  expect_vector((serializedPickle <- pickle(integerVector)))
  unserializedPickle <- unserialize(serializedPickle)
  expect_named(unserializedPickle[["objects"]], integerVectorAddress)
  expect_identical(unserializedPickle[["objects"]][[integerVectorAddress]], integerVector)

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(integerVector, unpickledValues)

  expect_vector((serializedPickle <- pickle(characterVector)))
  unserializedPickle <- unserialize(serializedPickle)
  expect_named(unserializedPickle[["objects"]], characterVectorAddress)
  expect_identical(unserializedPickle[["objects"]][[characterVectorAddress]], characterVector)

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(characterVector, unpickledValues)

  expect_vector((serializedPickle <- pickle(logicalVector)))
  unserializedPickle <- unserialize(serializedPickle)
  expect_named(unserializedPickle[["objects"]], logicalVectorAddress)
  expect_identical(unserializedPickle[["objects"]][[logicalVectorAddress]], logicalVector)

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(logicalVector, unpickledValues)
})
