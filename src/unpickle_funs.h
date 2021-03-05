#include <Rcpp.h>
using namespace Rcpp;

#ifndef __UNPICKLEFUNS__
#define __UNPICKLEFUNS__
RObject unpickle_tree_ (List& object,
                   std::unordered_map<String, RObject>& availableObjects,
                   int depth);
void apply_object_attributes_(RObject& object, List& attributes,
                              std::unordered_map<String, RObject>& availableObjects,
                              int depth);
void apply_object_attributes_(List& object, List& attributes,
                              std::unordered_map<String, RObject>& availableObjects,
                              int depth);
void populate_env_from_pickle_(RObject& environment,
                               List& pickleDefinition,
                               std::unordered_map<String, RObject>& availableObjects,
                               int depth);
#endif // __UNPICKLEFUNS__
