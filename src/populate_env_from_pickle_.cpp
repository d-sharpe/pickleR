#include <Rcpp.h>
#include <Rinternals.h>
#include "unpickle_funs.hpp"
using namespace Rcpp;

void populate_env_from_pickle_(RObject& environment,
                               List& pickleDefinition,
                               std::unordered_map<String, RObject>& availableObjects,
                               int depth) {

  Environment envToPopulate = as<Environment>(environment);

  if (!pickleDefinition.containsElementNamed("subObjects")) {
    print(pickleDefinition);
  }

  List subobjectDefinitions = pickleDefinition["subObjects"];

  for (List::iterator subObjectDef = subobjectDefinitions.begin();
       subObjectDef != subobjectDefinitions.end(); subObjectDef++) {

    List subObjectDefinition = *subObjectDef;

    String subObjectType = subObjectDefinition["Type"];

    if (subObjectType == "pickleEnvActiveBinding") {

      List bindingFunctionDefinition = subObjectDefinition["FunctionDefinition"];

      String subObjectName = bindingFunctionDefinition["objectLabel"];

      SEXP bindingFunction = unpickle_tree_(bindingFunctionDefinition, availableObjects, depth + 1);

      Symbol bindingFunctionSymbol(subObjectName);

      R_MakeActiveBinding(bindingFunctionSymbol, bindingFunction, envToPopulate);

    } else {

      String subObjectName = subObjectDefinition["objectLabel"];

      envToPopulate.assign(subObjectName, unpickle_tree_(subObjectDefinition, availableObjects, depth + 1));
    }
  }
}
