#' @title Serialize object maintaining internal object references
#' @description Recurses through the object hierarchy storing unique elements
#'     and the information necessary to reconstitute the object.
#' @param object The object to 'pickle'.
#'
#' @param connection an open connection, a string indicating the desired filename
#'     or NULL. If NULL returns a raw vector.
#' @param compress if connection is a string, and compress is TRUE, opens a gzfile
#'     to store the pickled object data to. If compress is FALSE no compression is applied.
#'     Compress can also be one of bzip, xz, gzip to indicate the type of compression to apply.
#' @param ... arguments passed to or from other methods.
#' @return NULL unless connection = NULL, when the result is returned in a raw vector.
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
            gzfile(connection, open = "wb", ...)
          } else {
            file(connection, open = "wb", ...)
          }
        } else {
          switch(
            compress,
            bzip2 = bzfile(connection, open = "wb", ...),
            xz = xzfile(connection,
                        open = "wb", ...),
            gzip = gzfile(connection, open = "wb", ...),
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
                connection = con,
                xdr = FALSE,
                ascii = FALSE)
    } else {
      serialize(object = pickle_(object),
                connection = NULL,
                xdr = FALSE,
                ascii = FALSE)
    }
  }
