# Internal package state shared across the current process (and inherited by
# forked child processes such as those created by parallel::mclapply).
.pickleR_state <- new.env(parent = emptyenv())
.pickleR_state$callback <- NULL

#' Register a global pickle callback
#'
#' Registers a function to be called at the end of every \code{\link{pickle}}
#' invocation.  The callback receives the original object as its first
#' argument.  Because the environment is inherited by forked child processes
#' (e.g. those created by \code{parallel::mclapply}), any callback registered
#' before forking is automatically available in every worker.
#'
#' The callback is invoked on a best-effort basis: if it signals an error the
#' error is caught, a \code{\link[base]{warning}} is issued, and \code{pickle}
#' continues normally.
#'
#' @param fun A function (or \code{NULL} to remove the current callback).  If
#'   a function, it must accept at least one argument (the pickled object).
#' @return The previously registered callback, invisibly.
#' @export
set_pickle_callback <- function(fun) {
  if (!is.null(fun) && !is.function(fun)) {
    stop("'fun' must be a function or NULL")
  }
  old <- .pickleR_state$callback
  .pickleR_state$callback <- fun
  invisible(old)
}

#' Retrieve the current global pickle callback
#'
#' @return The currently registered callback function, or \code{NULL} if none
#'   has been registered.
#' @export
get_pickle_callback <- function() {
  .pickleR_state$callback
}

#' Clear the global pickle callback
#'
#' Equivalent to \code{set_pickle_callback(NULL)}.
#'
#' @return The previously registered callback, invisibly.
#' @export
clear_pickle_callback <- function() {
  set_pickle_callback(NULL)
}
