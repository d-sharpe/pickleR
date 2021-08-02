#' @title Deserialize pickle data recreating object structure including
#'     internal object references
#' @description Uses serialized pickle generated data to reconstitute an object
#'     of the same structure as was originally pickled. Internal shared object
#'     references are maintained in the new object.
#' @param connection an open connection, a string indicating the name of source file
#'     or a raw vector.
#' @param ... arguments passed to or from other methods.
#' @return NULL unless connection = NULL, when the result is returned in a raw vector.
#' @export
unpickle <-
  function(connection, ...) {
    # Setup connection to file if passed
    if (is.character(connection)) {
      if (!file.exists(connection)) {
        stop("Specified file does not exist: ", connection)
      }
      file.info(connection)

      con <- gzfile(connection, "rb", ...)
      on.exit({
        close(con)
      }, add = TRUE)
    } else if (inherits(connection, "connection"))  {
      if (inherits(connection, "url")) {
        con <- gzcon(connection, ...)
        on.exit({
          close(con)
        }, add = TRUE)
      }
    } else {
      con <- connection
    }

    pickleDefinition <- unserialize(con)

    if (!is(pickleDefinition, "pickleDefinition")) {
      stop("Specified connection does not contain a valid pickle definition")
    }

    if (length(pickleDefinition[["requiredPackages"]])) {
      unavailablePackages <-
        !(pickleDefinition[["requiredPackages"]] %in% installed.packages()[, 1])

      if (any(unavailablePackages)) {
        stop(
          "Some dependencies are not available: ",
          paste0(pickleDefinition[["requiredPackages"]][unavailablePackages], collapse = ", ")
        )
      }
    }

    unpickle_(pickleDefinition)
  }
