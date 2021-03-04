#include <Rcpp.h>
#include <Rinternals.h>
#include "unpickle_funs.hpp"
using namespace Rcpp;

void populate_env_from_pickle_(RObject& environment,
                               List& pickleDefinition,
                               std::unordered_map<String, RObject>& availableObjects,
                               int depth) {

  Environment envToPopulate = as<Environment>(environment);

  List subobjectDefinitions = pickleDefinition["subObjects"];

  List subObjectDefinition, bindingFunctionDefinition;
  String subObjectType;
  String subObjectName;
  SEXP bindingFunction;

  for (List::iterator subObjectDef = subobjectDefinitions.begin();
       subObjectDef != subobjectDefinitions.end(); subObjectDef++) {

    subObjectDefinition = *subObjectDef;

    subObjectType = as<String>(subObjectDefinition["Type"]);

    if (subObjectType == "pickleEnvActiveBinding") {

      bindingFunctionDefinition = subObjectDefinition["FunctionDefinition"];

      subObjectName = as<String>(bindingFunctionDefinition["objectLabel"]);

      bindingFunction = unpickle_tree_(bindingFunctionDefinition, availableObjects, depth + 1);

      Symbol bindingFunctionSymbol(subObjectName);

      R_MakeActiveBinding(bindingFunctionSymbol, bindingFunction, envToPopulate);

    } else {

      subObjectName = as<String>(subObjectDefinition["objectLabel"]);

      envToPopulate.assign(subObjectName, unpickle_tree_(subObjectDefinition, availableObjects, depth + 1));
    }
  }
}
