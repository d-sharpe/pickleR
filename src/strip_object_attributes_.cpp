#include <Rcpp.h>
#include "pickle_funs.h"
using namespace Rcpp;

RObject strip_object_attributes_ (RObject& object) {

  // make a shallow duplicate of the object
  RObject shalDuplicateObject = Rf_shallow_duplicate(object);

  // strip the any src refs from the object - recursive for langsxp
  shalDuplicateObject = strip_src_refs(shalDuplicateObject);

  // get remaining attribute names
  CharacterVector objectAttributeNames = wrap(shalDuplicateObject.attributeNames());

  int numberOfObjectAttributes = objectAttributeNames.size();

  String attributeName;

  // loop through attributes and make null
  for (int i = 0; i < numberOfObjectAttributes; i++) {
    attributeName = objectAttributeNames[i];
    shalDuplicateObject.attr(attributeName) = R_NilValue;
  }

  return(shalDuplicateObject);

}
