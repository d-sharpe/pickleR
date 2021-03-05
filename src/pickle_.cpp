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

  // recurse through object tree pickling objects as we go
  List pickledObjectDefinition = pickle_tree_(object, "", seenObjects, seenAddresses, 1);

  // return the pickleDefinition
  return(List::create(_["objects"] = seenObjects,
                      _["pickleDefinition"] = pickledObjectDefinition));
}
