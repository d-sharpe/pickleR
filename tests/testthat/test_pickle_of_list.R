library(lobstr)

test_that("values in a list are perserved",{
  value <- 1:10

  valueList <- list(a = value,
                    b = value)

  expect_identical(obj_addr(value), obj_addr(valueList[["a"]]))

  expect_identical(obj_addr(valueList[["b"]]), obj_addr(valueList[["a"]]))

  expect_vector((pickledValue <- pickle(valueList)))

  # expect no error
  expect_error((unpickledValue <-
                  unpickle(pickledValue)), NA)

  expect_identical(unpickledValue, valueList)

  expect_identical(obj_addr(unpickledValue[["b"]]), obj_addr(unpickledValue[["a"]]))

  valueList <- list(a = value,
                    b = value,
                    c = NULL,
                    NULL)

  expect_vector((pickledValue <- pickle(valueList)))

  # expect no error
  expect_error((unpickledValue <-
                  unpickle(pickledValue)), NA)

  expect_identical(unpickledValue, valueList)




})
