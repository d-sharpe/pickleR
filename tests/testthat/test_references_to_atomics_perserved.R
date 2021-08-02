library(lobstr)

test_that("references to atomic objects are perserved in larger objects", {
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
})

test_that("references to non-atomic objects are perserved in larger objects",
          {
            value <- 1:10

            valueList <- list(a = value,
                              b = value)

            nestedValueList <- list(a = value,
                                    b = valueList)

            expect_identical(obj_addr(value), obj_addr(nestedValueList[["a"]]))

            expect_identical(obj_addr(valueList), obj_addr(nestedValueList[["b"]]))

            expect_identical(obj_addr(nestedValueList[["a"]]), obj_addr(nestedValueList[["b"]][["a"]]))

            expect_vector((pickledValue <- pickle(nestedValueList)))

            # expect no error
            expect_error((unpickledValue <-
                            unpickle(pickledValue)), NA)

            expect_identical(unpickledValue, nestedValueList)

            expect_identical(obj_addr(unpickledValue[["b"]][["a"]]), obj_addr(unpickledValue[["a"]]))

            expect_identical(obj_addr(unpickledValue[["b"]][["b"]]), obj_addr(unpickledValue[["a"]]))
          })
