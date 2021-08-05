#include <Rcpp.h>
#include "unpickle_funs.h"
using namespace Rcpp;

void apply_object_attributes_(RObject& object,
                              List& attributes,
                              std::unordered_map<std::string, RObject>& availableObjects,
                              int depth) {
  // extract list of attributes
  List attributeListToApply = attributes["Attributes"];

  // get number of attributes for loop
  int numberOfAttributes = attributeListToApply.length();

  // only loop if there are attributes
  if (numberOfAttributes > 0) {
    // allocate attribute name
    std::string attributeName;
    // allocate attribute definition list i.e. a pickle definition
    List attributeDefinition;

    for (int i = 0; i < numberOfAttributes; i++) {

      attributeDefinition = attributeListToApply[i];

      attributeName = as<std::string>(attributeDefinition["objectLabel"]);
      // unpickle the attribute and save to object
      object.attr(attributeName) = unpickle_tree_(attributeDefinition, availableObjects, depth + 1);
    }
  }
}

void apply_object_attributes_(List& object,
                              List& attributes,
                              std::unordered_map<std::string, RObject>& availableObjects,
                              int depth) {
  // extract list of attributes
  List attributeListToApply = attributes["Attributes"];

  // get number of attributes for loop
  int numberOfAttributes = attributeListToApply.length();

  // only loop if there are attributes
  if (numberOfAttributes > 0) {
    // allocate attribute name
    std::string attributeName;
    // allocate attribute definition list i.e. a pickle definition
    List attributeDefinition;

    for (int i = 0; i < numberOfAttributes; i++) {

      attributeDefinition = attributeListToApply[i];

      attributeName = as<std::string>(attributeDefinition["objectLabel"]);

      // unpickle the attribute and save to object
      object.attr(attributeName) = unpickle_tree_(attributeDefinition, availableObjects, depth + 1);
    }
  }
}
