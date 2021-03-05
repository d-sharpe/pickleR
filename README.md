
<!-- README.md is generated from README.Rmd. Please edit that file -->

# pickleR

<!-- badges: start -->

[![R-CMD-check](https://github.com/d-sharpe/pickleR/workflows/R-CMD-check/badge.svg)](https://github.com/d-sharpe/pickleR/actions)
[![test-coverage](https://github.com/d-sharpe/pickleR/workflows/test-coverage/badge.svg)](https://github.com/d-sharpe/pickleR/actions)
<!-- badges: end -->

*pickleR* provides for the serializing and de-serializing of an R object
structure, similar to that seen in the Python module
[pickle](https://docs.python.org/3/library/pickle.html). *pickleR*
traverses the object hierarchy, keeping track of any sub-objects it has
already encountered so that they will not be stored again. Therefore,
recursive, nested and shared objects are handled by *pickleR*.

The unpickling process reconstitutes the object with shared objects and
recursions intact.

## Installation

You can install the development version from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("d-sharpe/pickleR")
```

## Usage

``` r
library(pickleR)
library(lobstr)

valueVector <- rnorm(1000)

## memory address of vector
obj_addr(valueVector)
#> [1] "0x564635cdfef0"

## object memory size
obj_size(valueVector)
#> 8,048 B

listOfValues <-
  list(valueVector,
       valueVector)

## Each element of the listOfValues points to the same memory address of the
## original valueVector

lobstr::sxp(x = listOfValues)
#> [1:0x564635271908] <VECSXP[2]> (named:4)
#>   [2:0x564635cdfef0] <REALSXP[1000]> (named:6)
#>   [2:0x564635cdfef0]

## listOfValues memory size
obj_size(listOfValues)
#> 8,112 B

RListOfValues <-
  unserialize(serialize(listOfValues, connection = NULL))

## Each element of the RListOfValues points to a new vector i.e. shared object
## reference were not maintained

lobstr::sxp(x = RListOfValues)
#> [1:0x5646343b33d8] <VECSXP[2]> (named:4)
#>   [2:0x564636f0f810] <REALSXP[1000]> (named:1)
#>   [3:0x564636ce87d0] <REALSXP[1000]> (named:1)

## RListOfValues memory size
obj_size(RListOfValues)
#> 16,160 B

# The serialized object consumes more memory than the original

pickleListOfValues <-
  unpickle(pickle(listOfValues))

## Each element of the pickleListOfValues points to the same memory 
## address i.e. shared object

lobstr::sxp(x = pickleListOfValues)
#> [1:0x5646366b4dd8] <VECSXP[2]> (named:10)
#>   [2:0x5646372ac090] <REALSXP[1000]> (named:9)
#>   [2:0x5646372ac090]

## pickleListOfValues memory size
obj_size(pickleListOfValues)
#> 8,112 B

# The pickled object consumes the same memory as the original
```
