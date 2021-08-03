test_that("environment structures are perserved", {
  testEnvironment <- new.env(parent = globalenv())

  testEnvironment$firstValue <- 1L

  testEnvironment$selfReference <- testEnvironment

  testFunction <- function(value) {
    if (missing(value)) {
      return(firstValue)
    } else {
      firstValue <- value
    }
  }

  environment(testFunction) <- testEnvironment

  makeActiveBinding("activeBinding", fun = testFunction, env = testEnvironment)

  lockEnvironment(testEnvironment)

  expect_vector((pickledEnv <- pickle(testEnvironment)))

  # expect no error
  expect_error((unpickledEnv <-
                  unpickle(pickledEnv)), NA)

  expect_type(unpickledEnv, "environment")

  expect_equal(parent.env(unpickledEnv), globalenv())

  expect_true(environmentIsLocked(unpickledEnv))

  expect_vector((pickledFunction <- pickle(testFunction)))

  # expect no error
  expect_error((unpickledFunction <-
                  unpickle(pickledFunction)), NA)

  expect_true(environmentIsLocked(environment(unpickledFunction)))

})
