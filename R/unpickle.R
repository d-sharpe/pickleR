#' @export
unpickle <-
  function(connection, ...) {
    # Setup connection to file if passed
    if (is.character(connection)) {
      con <- gzfile(connection, "rb")
      on.exit(close(con))
    } else if (inherits(connection, "connection"))  {
      if (inherits(connection, "url")) {
        con <- gzcon(connection)
        on.exit(close(con))
      }
    } else {
      con <- connection
    }

    pickleDefinition <- unserialize(con)

    unpickle_(pickleDefinition)
  }
