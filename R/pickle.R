#' @export
pickle <-
  function(object,
           connection = NULL,
           compress = TRUE,
           ...) {
    if (!is.null(connection)) {
      if (is.character(connection)) {
        con <- if (is.logical(compress)) {
          if (compress) {
            gzfile(connection, open = "wb")
          } else {
            file(connection, open = "wb")
          }
        } else {
          switch(
            compress,
            bzip2 = bzfile(connection, open = "wb"),
            xz = xzfile(connection,
                        open = "wb"),
            gzip = gzfile(connection, open = "wb"),
            stop("invalid 'compress' argument: ",
                 compress)
          )
        }

        on.exit(close(con))
      } else {
        con <- connection
      }

      # pickle the object and serialize the output result to the connection. If
      # connection is null, raw vector returned
      serialize(object = pickle_(object),
                connection = con)
    } else {
      serialize(object = pickle_(object),
                connection = NULL, xdr = FALSE, ascii = FALSE)
    }
  }
