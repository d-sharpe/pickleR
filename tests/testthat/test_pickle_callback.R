test_that("pickle callback is called once per pickle", {
  callCount <- 0L
  on.exit(clear_pickle_callback(), add = TRUE)

  set_pickle_callback(function(obj) {
    callCount <<- callCount + 1L
  })

  pickle(1:10)
  expect_equal(callCount, 1L)

  pickle("hello")
  expect_equal(callCount, 2L)
})

test_that("pickle callback receives the pickled object", {
  received <- NULL
  on.exit(clear_pickle_callback(), add = TRUE)

  set_pickle_callback(function(obj) {
    received <<- obj
  })

  value <- list(a = 1, b = "test")
  pickle(value)

  expect_identical(received, value)
})

test_that("pickle callback error issues a warning but does not stop pickle", {
  on.exit(clear_pickle_callback(), add = TRUE)

  set_pickle_callback(function(obj) {
    stop("intentional callback error")
  })

  # pickle must succeed even though the callback errors
  expect_warning(
    result <- pickle(42L),
    "pickle callback error: intentional callback error"
  )

  expect_type(result, "raw")
  expect_identical(unpickle(result), 42L)
})

test_that("get_pickle_callback returns registered callback", {
  on.exit(clear_pickle_callback(), add = TRUE)

  expect_null(get_pickle_callback())

  myFun <- function(obj) invisible(NULL)
  set_pickle_callback(myFun)

  expect_identical(get_pickle_callback(), myFun)
})

test_that("clear_pickle_callback removes callback", {
  on.exit(clear_pickle_callback(), add = TRUE)

  set_pickle_callback(function(obj) invisible(NULL))
  clear_pickle_callback()

  expect_null(get_pickle_callback())
})

test_that("set_pickle_callback rejects non-function values", {
  expect_error(set_pickle_callback(42), "'fun' must be a function or NULL")
  expect_error(set_pickle_callback("not a function"), "'fun' must be a function or NULL")
})

test_that("callback is NULL by default (no spurious calls)", {
  on.exit(clear_pickle_callback(), add = TRUE)
  clear_pickle_callback()

  # No warning expected when no callback is registered
  expect_no_warning(pickle(1:5))
})
