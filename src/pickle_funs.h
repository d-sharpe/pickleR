#include <Rcpp.h>
using namespace Rcpp;

#ifndef __PICKLEFUNS__
#define __PICKLEFUNS__
List pickle_tree_ (RObject& object,
                   String objectLabel,
                   std::unordered_map<String, RObject>& seenObjects,
                   std::unordered_set<String>& seenAddresses,
                   std::unordered_set<String>& requiredPackages,
                   int depth);
String object_address_(RObject& object);
List extract_object_attributes_ (RObject& object,
                                 std::unordered_map<String, RObject>& seenObjects,
                                 std::unordered_set<String>& seenAddresses,
                                 std::unordered_set<String>& requiredPackages);
RObject strip_src_refs(RObject object);
RObject strip_object_attributes_ (RObject& object);
SEXP get_binding_function_ (String name, Environment& environment);
Rboolean is_binding_function_ (String name, Environment& environment);
#endif // __PICKLEFUNS__
