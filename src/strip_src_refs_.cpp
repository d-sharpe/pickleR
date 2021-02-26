#include <Rcpp.h>
using namespace Rcpp;

RObject strip_src_refs(RObject object) {

  // if object is a closure grab body and strip
  if (TYPEOF(object) == CLOSXP) {

    RObject objectBody = BODY(object);

    return(strip_src_refs(objectBody));
  }

  // if object is not a language type return
  if (TYPEOF(object) != LANGSXP && TYPEOF(object) != EXPRSXP) {
    return(object);
  }

  // create a shallow duplicate of object to protect from editing original source
  RObject shalDuplicateObject = Rf_shallow_duplicate(object);

  // remove the src attributes
  shalDuplicateObject.attr("srcref") = R_NilValue;
  shalDuplicateObject.attr("wholeSrcref") = R_NilValue;
  shalDuplicateObject.attr("srcfile") = R_NilValue;

  if (CAR(shalDuplicateObject) ==  Rf_install("function")) {
    SETCDR(CDR(CDR(shalDuplicateObject)), R_NilValue);
  }

  // temporary holder for each "line" of language object
  RObject languageElement = shalDuplicateObject;

  // loop the language object until Nilvalue encountered i.e. end of pairlist
  while (languageElement != R_NilValue) {

    // replace first element with stripped first element
    SETCAR(languageElement, strip_src_refs(CAR(languageElement)));

    // set language object holder to remaining elements
    languageElement = CDR(languageElement);
  }

  // return stripped copy
  return(shalDuplicateObject);
}
