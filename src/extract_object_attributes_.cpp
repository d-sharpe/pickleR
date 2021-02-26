#include <Rcpp.h>
#include "pickle_funs.hpp"
using namespace Rcpp;

List extract_object_attributes_ (RObject& object,
                                 std::unordered_map<String, RObject>& seenObjects,
                                 std::unordered_set<String>& seenAddresses) {
  RObject strippedObject = strip_src_refs(object);
  CharacterVector objectAttributeNames = wrap(strippedObject.attributeNames());

  int numberOfObjectAttributes = objectAttributeNames.size();

  List objectAttributeList (numberOfObjectAttributes);

  for (int i = 0; i < numberOfObjectAttributes; i++) {
    String attributeName = String(objectAttributeNames[i]);
    RObject attribute = strippedObject.attr(attributeName);
    objectAttributeList[i] = pickle_tree_ (attribute, attributeName, seenObjects, seenAddresses, 1);
  }

  return(List::create(_["Attributes"] = objectAttributeList,
                      _["Type"] = "pickleAttributeList"));
}
