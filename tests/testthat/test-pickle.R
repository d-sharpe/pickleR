library(pryr)

numericVector <- runif(100)
numericVectorAddress <- address(numericVector)

integerVector <- 1L:100L
integerVectorAddress <- address(integerVector)

characterVector <- sample(LETTERS, 100, replace = TRUE)
characterVectorAddress <- address(characterVector)

logicalVector <- sample(c(TRUE, FALSE), 100, replace = TRUE)
logicalVectorAddress <- address(logicalVector)

test_that("pickle of base types works", {

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

  mixedList <-
    list(ItemA = numericVector,
         ItemB = characterVector,
         ItemC = integerVector)

  expect_vector((serializedPickle <- pickle(mixedList)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(mixedList, unpickledValues)
})

test_that("pickle of object with attributes works", {

  factorVector <-
    factor(sample(LETTERS[1:5], 100, replace = T))

  factorVectorAddress <- address(factorVector)

  expect_vector((serializedPickle <- pickle(factorVector)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(factorVector, unpickledValues)
})

test_that("pickle of an environment works", {

  env <-
    new.env(parent = emptyenv())

  env$a <-
    numericVector

  env$b <-
    characterVector

  expect_vector((serializedPickle <- pickle(env)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(names(env), names(unpickledValues))

  expect_identical(env$a, unpickledValues$a)

  expect_identical(env$b, unpickledValues$b)


})


test_that("pickle of a recursive environment works", {

  temp_env <-
    new.env(parent = emptyenv())

  temp_env$a <-
    numericVector

  temp_env$b <-
    characterVector

  temp_env$c <-
    list(temp_env)

  expect_vector((serializedPickle <- pickle(temp_env)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(names(temp_env), names(unpickledValues))

  expect_identical(temp_env$a, unpickledValues$a)

  expect_identical(temp_env$b, unpickledValues$b)

  expect_identical(unpickledValues$c, list(unpickledValues))

  fun <-
    function(...) { return(50)}

  makeActiveBinding("d", fun, temp_env)

  expect_vector((serializedPickle <- pickle(temp_env)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_setequal(names(temp_env), names(unpickledValues))

  expect_identical(temp_env$a, unpickledValues$a)

  expect_identical(temp_env$b, unpickledValues$b)

  expect_identical(unpickledValues$c, list(unpickledValues))

  expect_identical(unpickledValues$d, 50)

  expect_true(bindingIsActive("d", unpickledValues))

  })


test_that("pickle of a function works",  {

  fun <-
    function(a) {
      a + 1
    }

  expect_vector((serializedPickle <- pickle(fun)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(formals(fun), formals(unpickledValues))

  expect_identical(body(fun), body(unpickledValues))


  environment(fun) <- new.env(parent = emptyenv())

  expect_vector((serializedPickle <- pickle(fun)))

  unpickledValues <-
    unpickle(serializedPickle)

  expect_identical(formals(fun), formals(unpickledValues))

  expect_identical(body(fun), body(unpickledValues))
})
