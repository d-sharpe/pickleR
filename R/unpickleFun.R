unpickleFun <- function(formals, function_body, environment) {
  # create and empty function
  emptyFunction <-  function() {}

  # replace formal arguments
  formals(emptyFunction) <-
    formals

  # replace body of the function
  body(emptyFunction) <-
    substitute(function_body)

  # replace function run environment
  environment(emptyFunction) <-
    environment

  return(emptyFunction)
}
