## ---- include = FALSE---------------------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>",
  message = FALSE
)

## -----------------------------------------------------------------------------
x <- 1:100

list_of_x <- list(x, x, x)

## -----------------------------------------------------------------------------
library(pryr)

pryr::inspect(x)

pryr::inspect(list_of_x)

## -----------------------------------------------------------------------------
restored_list_of_x <-
  unserialize(serialize(list_of_x, connection = NULL))

pryr::inspect(restored_list_of_x)

identical(list_of_x, restored_list_of_x)

## -----------------------------------------------------------------------------
pryr::object_size(x)

pryr::object_size(list_of_x)
## 80 B for list structure

pryr::object_size(restored_list_of_x)
## 3 * bytes of x + 80 B list structure

## ----setup--------------------------------------------------------------------
library(pickleR)

pickleR_restored_list_of_x <-
  unpickle(pickle(list_of_x, connection = NULL))

pryr::inspect(pickleR_restored_list_of_x)

identical(list_of_x, pickleR_restored_list_of_x)

## -----------------------------------------------------------------------------
pryr::object_size(pickleR_restored_list_of_x)

## -----------------------------------------------------------------------------
library(R6)

classA <-
  R6Class(
    classname = "classA",
    class = TRUE,
    cloneable = TRUE,
    public = list(normalValue = rnorm(1),
                  uniformValue = runif(1),
                  normalRange = function(mean, sd, n = 10) {
                    return(rnorm(n = n, mean = mean, sd = sd))
                  },
                  uniformRange = function(min, max, n = 10) {
                    return(runif(n = n, min = min, max = max))
                  })
  )

instance_of_classA <-
  classA$new()

pryr::object_size(instance_of_classA)

pryr::inspect(instance_of_classA$normalRange)[["children"]][["body"]][["address"]]

second_instance_of_classA <-
  classA$new()

pryr::inspect(second_instance_of_classA$normalRange)[["children"]][["body"]][["address"]]

## -----------------------------------------------------------------------------
restored_instance_of_classA <-
  unserialize(serialize(instance_of_classA, connection = NULL))


pryr::object_size(restored_instance_of_classA)

pryr::inspect(restored_instance_of_classA$normalRange)[["children"]][["body"]][["address"]]


## -----------------------------------------------------------------------------

pickleR_restored_instance_of_classA <-
    unpickle(pickle(instance_of_classA, connection = NULL))

pryr::object_size(pickleR_restored_instance_of_classA)

pryr::inspect(pickleR_restored_instance_of_classA$normalRange)[["children"]][["body"]][["address"]]

## -----------------------------------------------------------------------------
list_of_classA_instances <-
  lapply(X = 1:10,
         FUN = function(i) {
           classA$new()
         })

pryr::object_size(list_of_classA_instances)

## -----------------------------------------------------------------------------
restored_list_of_classA_instances <-
  unserialize(serialize(list_of_classA_instances, connection = NULL))

pryr::object_size(restored_list_of_classA_instances)


## -----------------------------------------------------------------------------
pickleR_restored_list_of_classA_instances <-
  unpickle(pickle(list_of_classA_instances, connection = NULL))

pryr::object_size(pickleR_restored_list_of_classA_instances)



