#include <Rcpp.h>
using namespace Rcpp;

#ifndef __PICKLEFUNS__
#define __PICKLEFUNS__
List pickle_tree_ (RObject& object,
                   std::string objectLabel,
                   std::unordered_map<std::string, RObject>& seenObjects,
                   std::unordered_set<std::string>& seenAddresses,
                   std::unordered_set<std::string>& requiredPackages,
                   int depth);
std::string object_address_(RObject& object);
List extract_object_attributes_ (RObject& object,
                                 std::unordered_map<std::string, RObject>& seenObjects,
                                 std::unordered_set<std::string>& seenAddresses,
                                 std::unordered_set<std::string>& requiredPackages);
RObject strip_src_refs(RObject object);
RObject strip_object_attributes_ (RObject& object);
SEXP get_binding_function_ (std::string name, Environment& environment);
Rboolean is_binding_function_ (std::string name, Environment& environment);
Rboolean is_environment_locked_ (Environment& environment);
#endif // __PICKLEFUNS__
