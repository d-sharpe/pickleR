#include <Rcpp.h>
#include "unpickle_funs.hpp"
using namespace Rcpp;

// [[Rcpp::export]]
RObject unpickle_ (List pickleDefinition) {

  // create unordered map to hold unique objects
  std::unordered_map<String, RObject> availableObjects;

  List pickledObjects = pickleDefinition["objects"];

  CharacterVector pickledObjectAddresses = pickledObjects.names();

  for (CharacterVector::iterator objectAddress = pickledObjectAddresses.begin();
       objectAddress != pickledObjectAddresses.end();
       objectAddress++) {

    String currentObjectAddress = String(*objectAddress);
    availableObjects.insert(std::pair<String, RObject>(currentObjectAddress, pickledObjects[currentObjectAddress]));
  }

  List pickledObjectDefintion = pickleDefinition["pickleDefinition"];

  // return the unpickled Object
  return(unpickle_tree_(pickledObjectDefintion, availableObjects, 1));
}
