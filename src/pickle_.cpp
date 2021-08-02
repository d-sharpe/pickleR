#include <Rcpp.h>
#include "pickle_funs.h"
using namespace Rcpp;

// [[Rcpp::export]]
List pickle_ (RObject object) {

  // create unordered map to hold unique objects
  std::unordered_map<String, RObject> seenObjects;

  // create unordered set to hold seen addresses that we don't want
  // to store the object for directly i.e. a list
  std::unordered_set<String> seenAddresses;

  // create unordered set to hold seen package environments that will be required
  // to recreate the object
  std::unordered_set<String> requiredPackages;

  // recurse through object tree pickling objects as we go
  List pickledObjectDefinition = pickle_tree_(object, "", seenObjects, seenAddresses, requiredPackages, 1);

  int numberOfRequiredPackages = requiredPackages.size();
  CharacterVector requiredPackageNames(numberOfRequiredPackages);

  std::unordered_set<String>::const_iterator currentPackage = requiredPackages.begin(),
    lastPackage = requiredPackages.end();

  for (int i = 0; currentPackage != lastPackage; currentPackage++, i++) {
    requiredPackageNames[i] = *currentPackage;
  }

  // create the pickleDefintion list
  List pickleDefinition = List::create(_["objects"] = seenObjects,
                                       _["requiredPackages"] = requiredPackageNames,
                                       _["pickleDefinition"] = pickledObjectDefinition);

  pickleDefinition.attr("class") = "pickleDefinition";

  // return the pickleDefinition
  return(pickleDefinition);
}
