#include <Rcpp.h>
#include "unpickle_funs.h"
using namespace Rcpp;

// [[Rcpp::export]]
RObject unpickle_ (List pickleDefinition) {

  List pickledObjects = pickleDefinition["objects"];

  // create unordered map to hold unique objects
  std::unordered_map<std::string, RObject> availableObjects;

  CharacterVector pickledObjectAddresses = pickledObjects.names();

  int numberOfObjects = pickledObjectAddresses.length();

  std::string currentObjectAddress;
  RObject currentObject;
  for (int i = 0; i < numberOfObjects; i++) {

    currentObjectAddress = pickledObjectAddresses[i];
    currentObject = pickledObjects[i];
    availableObjects.insert(std::pair<std::string, RObject>(currentObjectAddress, currentObject));
  }

  List pickledObjectDefintion = pickleDefinition["pickleDefinition"];

  // return the unpickled Object
  return(unpickle_tree_(pickledObjectDefintion, availableObjects, 1));
}
