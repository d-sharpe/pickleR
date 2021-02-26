#include <Rcpp.h>
#include "pickle_funs.hpp"
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

  // create list to store the unique objects
  List uniqueObjectList(seenObjects.size());
  // create list to store object "names" (addresses)
  CharacterVector uniqueObjectAddresses(seenObjects.size());

  // create iterators to loop over seenObject map
  std::unordered_map<String, RObject>::const_iterator currentObject = seenObjects.begin(),
                                                      lastObject = seenObjects.end();

  // loop with iterator and create R list of unique objects and corresponding addresses
  for (R_xlen_t i = 0; currentObject != lastObject; currentObject++, i++) {
    uniqueObjectList[i] = currentObject->second;
    uniqueObjectAddresses[i] = currentObject->first;
  }

  // apply object addresses as names to unqiue object list
  uniqueObjectList.attr("names") = uniqueObjectAddresses;

  // return the pickleDefinition
  return(List::create(_["objects"] = uniqueObjectList,
                      _["pickleDefinition"] = pickledObjectDefinition));
}
