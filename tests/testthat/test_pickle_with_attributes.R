library(lobstr)

test_that("object attributes are perserved on atomic types", {
  value <- runif(1)

  # single attribute
  attr(value, which = "class") <- "TestClass"

  expect_vector((pickledValue <- pickle(value)))

  expect_vector((unpickledValue <-
                   unpickle(pickledValue)))

  expect_identical(unpickledValue, value)

  expect_identical(attr(value, which = "class"), "TestClass")

  # multiple attribute
  attr(value, which = "secondAttr") <- TRUE

  expect_vector((pickledValue <- pickle(value)))

  expect_vector((unpickledValue <-
                   unpickle(pickledValue)))

  expect_identical(unpickledValue, value)

  expect_identical(attr(value, which = "class"), "TestClass")

  expect_identical(attr(value, which = "secondAttr"), TRUE)
})

test_that("object attributes are perserved on list", {
  value <- list(FirstElement = LETTERS,
                SecondElement = 1:10)

  # single attribute
  attr(value, which = "class") <- "TestClass"

  expect_vector((pickledValue <- pickle(value)))

  # expect no error
  expect_error((unpickledValue <-
                  unpickle(pickledValue)), NA)

  expect_identical(unpickledValue, value)

  expect_identical(attr(value, which = "class"), "TestClass")

  # multiple attribute
  attr(value, which = "secondAttr") <- TRUE

  expect_vector((pickledValue <- pickle(value)))

  # expect no error
  expect_error((unpickledValue <-
                  unpickle(pickledValue)), NA)

  expect_identical(unpickledValue, value)

  expect_identical(attr(value, which = "class"), "TestClass")

  expect_identical(attr(value, which = "secondAttr"), TRUE)
})
