test_that("pickling of package function allows function recreation", {
  testFunction <-
    function(a, b = 5) {
      return(a + b)
    }

  expect_vector((pickledFunction <- pickle(testFunction)))

  # expect no error
  expect_error((unpickledFunction <-
                  unpickle(pickledFunction)), NA)

  expect_equal(unpickledFunction(a = 10, b = 11), 21)

  # pickle a function from within the testthat package
  expect_vector((pickledFunction <- pickle(test_example)))

  # expect no error
  expect_error((unpickledFunction <-
                  unpickle(pickledFunction)), NA)

  # environment of built-in or package environments should be perserved
  expect_identical(environment(test_example),
                   environment(unpickledFunction))

  expect_identical(formals(test_example), formals(unpickledFunction))
})
