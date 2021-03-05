#include <Rcpp.h>
#include "pickle_funs.h"
using namespace Rcpp;

List extract_object_attributes_ (RObject& object,
                                 std::unordered_map<String, RObject>& seenObjects,
                                 std::unordered_set<String>& seenAddresses) {

  CharacterVector objectAttributeNames = wrap(object.attributeNames());

  int numberOfObjectAttributes = objectAttributeNames.size();

  List objectAttributeList (numberOfObjectAttributes);

  String attributeName;
  RObject attribute;

  for (int i = 0; i < numberOfObjectAttributes; i++) {
    attributeName = objectAttributeNames[i];
    attribute = object.attr(attributeName);
    objectAttributeList[i] = pickle_tree_ (attribute, attributeName, seenObjects, seenAddresses, 1);
  }

  return(List::create(_["Attributes"] = objectAttributeList,
                      _["Type"] = "pickleAttributeList"));
}
