#include <Rcpp.h>
#include <Rinternals.h>
#include "unpickle_funs.h"
using namespace Rcpp;

void populate_env_from_pickle_(RObject& environment,
                               List& pickleDefinition,
                               std::unordered_map<String, RObject>& availableObjects,
                               int depth) {

  // convert object to an environment
  Environment envToPopulate = as<Environment>(environment);

  // get the definitions of the subobjects
  List subobjectDefinitions = pickleDefinition["subObjects"];

  // allocate the loop variables
  List subObjectDefinition, bindingFunctionDefinition;
  String subObjectType, subObjectName;
  SEXP bindingFunction;

  for (List::iterator subObjectDef = subobjectDefinitions.begin();
       subObjectDef != subobjectDefinitions.end(); subObjectDef++) {

    subObjectDefinition = *subObjectDef;

    subObjectType = as<String>(subObjectDefinition["Type"]);

    // if the type of the sub object is an active binding
    if (subObjectType == "pickleEnvActiveBinding") {

      // get the definition of the bound function
      bindingFunctionDefinition = subObjectDefinition["FunctionDefinition"];

      subObjectName = as<String>(bindingFunctionDefinition["objectLabel"]);

      // unpickle the function
      bindingFunction = unpickle_tree_(bindingFunctionDefinition, availableObjects, depth + 1);

      Symbol bindingFunctionSymbol(subObjectName);

      // make the active binding
      R_MakeActiveBinding(bindingFunctionSymbol, bindingFunction, envToPopulate);

    } else {

      // if subobject is a value unpickle and assign
      subObjectName = as<String>(subObjectDefinition["objectLabel"]);

      envToPopulate.assign(subObjectName, unpickle_tree_(subObjectDefinition, availableObjects, depth + 1));
    }
  }
}
