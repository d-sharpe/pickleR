test_that("pickling builtin functions is preserved", {
  # BUILTINSXP - e.g. sum, c
  expect_vector((pickledSum <- pickle(sum)))
  expect_error((unpickledSum <- unpickle(pickledSum)), NA)
  expect_equal(unpickledSum(1:5), sum(1:5))

  # SPECIALSXP - e.g. `if`
  expect_vector((pickledIf <- pickle(`if`)))
  expect_error((unpickledIf <- unpickle(pickledIf)), NA)
})

test_that("pickling S4 objects is preserved", {
  setClass("TestS4", representation(x = "numeric", y = "character"))
  on.exit(removeClass("TestS4"), add = TRUE)

  obj <- new("TestS4", x = 3.14, y = "hello")

  expect_vector((pickled <- pickle(obj)))
  expect_error((unpickled <- unpickle(pickled)), NA)

  expect_s4_class(unpickled, "TestS4")
  expect_equal(unpickled@x, obj@x)
  expect_equal(unpickled@y, obj@y)
})
