#include <Rcpp.h>
#include "pickle_funs.h"
using namespace Rcpp;

List extract_object_attributes_ (RObject& object,
                                 std::unordered_map<std::string, RObject>& seenObjects,
                                 std::unordered_set<std::string>& seenAddresses,
                                 std::unordered_set<std::string>& requiredPackages) {
  // get attribute names
  CharacterVector objectAttributeNames = wrap(object.attributeNames());
  // get number of attributes
  int numberOfObjectAttributes = Rf_xlength(objectAttributeNames);

  // allocate list of length = number of attributes
  List objectAttributeList (numberOfObjectAttributes);

  // allocate for loop variables
  std::string attributeName;
  RObject attribute;

  for (int i = 0; i < numberOfObjectAttributes; i++) {
    attributeName = objectAttributeNames[i];
    attribute = object.attr(attributeName);

    // pickle the attribute and save to attribute list
    objectAttributeList[i] = pickle_tree_ (attribute, attributeName, seenObjects, seenAddresses, requiredPackages, 1);
  }

  return(List::create(_["Attributes"] = objectAttributeList,
                      _["Type"] = "pickleAttributeList"));
}
