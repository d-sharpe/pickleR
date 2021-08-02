test_that("pickles can be written to file", {
  value <- 1:10000

  expect_vector(pickle(value, connection = NULL))

  temporaryPickleFile <- tempfile()

  on.exit({
    unlink(temporaryPickleFile)
  }, add = TRUE)

  pickle(value, connection = temporaryPickleFile)

  expect_true(file.exists(temporaryPickleFile))

  expect_identical(unpickle(temporaryPickleFile), value)
})

test_that("pickle options must be valid", {
  value <- 1:10000

  temporaryPickleFile <- tempfile()

  on.exit({
    unlink(temporaryPickleFile)
  }, add = TRUE)

  expect_error(
    pickle(value,
           connection = temporaryPickleFile,
           compress = 5),
    "Invalid 'compress' argument: 5"
  )

  expect_error(
    pickle(value,
           connection = temporaryPickleFile,
           compress = "jzip"),
    "Invalid 'compress' argument: jzip"

  )

  # expect no error, NA disables compression
  expect_error(pickle(value,
                      connection = temporaryPickleFile,
                      compress = NA),
               NA)

  expect_equal({
    fileConnection <- file(temporaryPickleFile)

    returnValue <- summary(fileConnection)[["class"]]

    close(fileConnection)

    returnValue
  }, "file")

  # no error as gzip is valid compression
  expect_error(pickle(value,
                      connection = temporaryPickleFile,
                      compress = "gzip"),
               NA)

  expect_equal({
    fileConnection <- file(temporaryPickleFile)

    returnValue <- summary(fileConnection)[["class"]]

    close(fileConnection)

    returnValue
  }, "gzfile")

})

test_that("pickles can be read from file and values perserved", {
  value <- 1:10000

  temporaryPickleFile <- tempfile()

  on.exit({
    unlink(temporaryPickleFile)
  }, add = TRUE)

  expect_error(pickle(value,
                      connection = temporaryPickleFile,
                      compress = TRUE),
               NA)

  expect_identical(unpickle(temporaryPickleFile), value)

  expect_error(
    unpickle("a_file_that_does_not_exist.pck"),
    "Specified file does not exist: a_file_that_does_not_exist.pck"
  )

  randomDataFile <-
    tempfile()

  randomDataFileConnection <-
    file(randomDataFile, open = "wb")

  serialize(value, connection = randomDataFileConnection)

  # close connection to persist data to file
  close(randomDataFileConnection)

  expect_error(
    unpickle(randomDataFile),
    "Specified connection does not contain a valid pickle definition"
  )
})
