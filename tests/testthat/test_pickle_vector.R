test_that("numeric scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- runif(1)

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            # Generate a vector of values
            vectorValues <- runif(100000)

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)
          })

test_that("integer scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- 1L

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            # Generate a vector of values (1 to N, randomised order)
            vectorValues <- sample(100000, size = 100000)

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)
          })

test_that("character scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- "a string"

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            # Generate a vector of values
            vectorValues <- c("first string in a vector",
                              "second string in a vector",
                              "and so on")

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)
          })

test_that("logical scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- TRUE

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            # Generate a vector of values
            vectorValues <- c(T, F, T, F, F, T, T, F)

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)
          })

test_that("raw scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- as.raw((originalScalarValue <- 5L))

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            expect_identical(as.integer(unpickledValue), originalScalarValue)

            # Generate a vector of values
            vectorValues <-
              as.raw((originalVectorValue <- sample(255, size = 255)))

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)

            expect_identical(as.integer(unpickledValue), originalVectorValue)
          })

test_that("complex scalar and vector values are perserved",
          {
            # Generate a numeric scalar value (vector of length 1)
            scalarValue <- 2i ^ 4

            expect_vector((pickledValue <- pickle(scalarValue)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, scalarValue)

            # Generate a vector of values
            vectorValues <- 2i ^ (-3:3)

            expect_vector((pickledValue <- pickle(vectorValues)))

            expect_vector((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, vectorValues)
          })

test_that("NULL values are perserved",
          {
            nullValue <- NULL

            expect_vector((pickledValue <- pickle(nullValue)))

            expect_null((unpickledValue <-
                             unpickle(pickledValue)))

            expect_identical(unpickledValue, nullValue)
          })
