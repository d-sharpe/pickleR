#include <Rcpp.h>
#include "pickle_funs.hpp"
using namespace Rcpp;

RObject strip_object_attributes_ (RObject& object) {

  RObject shalDuplicateObject = Rf_shallow_duplicate(object);

  shalDuplicateObject = strip_src_refs(shalDuplicateObject);

  CharacterVector objectAttributeNames = wrap(shalDuplicateObject.attributeNames());

  int numberOfObjectAttributes = objectAttributeNames.size();

  for (int i = 0; i < numberOfObjectAttributes; i++) {
    String attributeName = String(objectAttributeNames[i]);
    shalDuplicateObject.attr(attributeName) = R_NilValue;
  }

  return(shalDuplicateObject);

}
