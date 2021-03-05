#include <Rcpp.h>
#include <Rinternals.h>
#include "unpickle_funs.h"
using namespace Rcpp;

RObject unpickle_tree_ (List& pickleDefinition,
                        std::unordered_map<String, RObject>& availableObjects,
                        int depth) {
  // allocate temp object and atribute list
  RObject object;
  List objectAttributes;

  // get type of pickle
  String pickleType = pickleDefinition["Type"];

  // if is pickle referring to a built-in R environment e.g. Global
  // return the environment
  if (pickleType == "pickleBaseEnv") {
    return(R_BaseEnv);
  }

  if (pickleType == "pickleGlobalEnv") {
    return(R_GlobalEnv);
  }

  if (pickleType == "pickleEmptyEnv") {
    return(R_EmptyEnv);
  }

  // if us pickle referring to a installed package environment
  // return that package environment
  if (pickleType == "picklePackageEnv" || pickleType == "pickleNamespaceEnv") {
    String namespaceName = pickleDefinition["Namespace"];
    Environment packageEnv = Environment::namespace_env(namespaceName);

    return(as<RObject>(packageEnv));
  }

  // if is pickle NULL type return a R NULL
  if (pickleType == "pickleNULL") {

    object = R_NilValue;

    objectAttributes = pickleDefinition["objectAttributes"];

    apply_object_attributes_(object, objectAttributes, availableObjects, depth + 1);

    return(object);
  }

  String objectAddress = pickleDefinition["objectAddress"];
  String objectLabel = pickleDefinition["objectLabel"];

  // if is a pickle then extract object from map of available objects
  if (pickleType == "pickle") {

    objectAttributes = pickleDefinition["objectAttributes"];

    object = availableObjects[objectAddress];

    apply_object_attributes_(object, objectAttributes, availableObjects, depth + 1);

    return(object);
  }

  // if is a pickle reference then object should be in map of available objects
  // so extract
  if (pickleType == "pickleReference") {
    return(availableObjects[objectAddress]);
  }

  // if is pickle list, loop over subObjects unpickling each and assigning to list
  if (pickleType == "pickleList") {
    List subObjectDefinitions = pickleDefinition["subObjects"];
    objectAttributes = pickleDefinition["objectAttributes"];

    int numberOfSubObjects = subObjectDefinitions.length();

    // if the list was not empty at time of pickle
    if (numberOfSubObjects > 0) {

      List subObjects (numberOfSubObjects);
      List subObjectDefinition;

      for (int subObjectDefinitionIndex = 0; subObjectDefinitionIndex < numberOfSubObjects; subObjectDefinitionIndex++) {

        subObjectDefinition = subObjectDefinitions[subObjectDefinitionIndex];

        subObjects[subObjectDefinitionIndex] = unpickle_tree_(subObjectDefinition, availableObjects, depth + 1);
      }

      apply_object_attributes_(subObjects, objectAttributes, availableObjects, depth + 1);

      // store in list of available objects
      availableObjects.insert(std::pair<String, RObject>(objectAddress, subObjects));

      // return list of objects
      return(subObjects);

    } else {

      // create an empty list
      List subObjects (0);

      apply_object_attributes_(subObjects, objectAttributes, availableObjects, depth + 1);

      // store in list of available objects
      availableObjects.insert(std::pair<String, RObject>(objectAddress, subObjects));

      // return list of objects
      return(subObjects);
    }
  }

  // if is a pickle function, unpickle the enclosing env, the formals and the args
  // then call to R function to generate R level function
  if (pickleType == "pickleFunction") {

    // get the pickle definition of the function environment
    List functionEnvInfo = pickleDefinition["objectEnvironment"];

    // get the type of the pickled enclosing environment
    String functionEnvironmentPickleType = functionEnvInfo["Type"];

    // allocate function
    RObject functionEnv;

    // get attributes to apply to function after generation
    objectAttributes = pickleDefinition["objectAttributes"];

    // should the elements of the enclosing env be populated after the function is created
    // important for R6 objects where the enclosing env may contain reference to the function
    // itself generating a recurive loop
    bool populatePostAssignment = false;

    // if the enclosing environment pickle is an custom environment (and has not been seen yet)
    if (functionEnvironmentPickleType == "pickleEnv") {

      // we will populate the environment elements after creating function
      populatePostAssignment = true;

      // get definition of parent of the enclosing environment
      List parentEnvInfo = functionEnvInfo["parentEnv"];

      // unpickle parent environment
      RObject parentEnv = unpickle_tree_(parentEnvInfo, availableObjects, depth + 1);

      // create a new environment with the parent as parent
      // keep env empty
      functionEnv = new_env(parentEnv);

      // get address of enclosing enviroment and save env to list of available objects
      String functionEnvironmentAddress = functionEnvInfo["objectAddress"];

      availableObjects.insert(std::pair<String, RObject>(functionEnvironmentAddress, functionEnv));
    } else {

      // if the enclosing env is a pickle reference of build-in R environment
      // unpickle the environment
      functionEnv = unpickle_tree_(functionEnvInfo, availableObjects, depth + 1);
    }

    // get function formal args and unpickle
    List functionFormalsInfo = pickleDefinition["objectFormals"];
    RObject functionFormals = unpickle_tree_(functionFormalsInfo, availableObjects, depth + 1);

    // get function body and unpickle
    List functionBodyInfo = pickleDefinition["objectBody"];
    RObject functionBody = unpickle_tree_(functionBodyInfo, availableObjects, depth + 1);

    // get ref to R level function to generate function object
    Environment pkg = Environment::namespace_env("pickleR");
    Function unpickleFun =pkg["unpickleFun"];

    // call to R function to generate R level function
    RObject unpickledFunction = unpickleFun(_["formals"] = functionFormals,
                                            _["function_body"] = functionBody,
                                            _["environment"] = functionEnv);

    // apply object attributes to function
    apply_object_attributes_(object, objectAttributes, availableObjects, depth + 1);

    // save function to list of available objects
    availableObjects.insert(std::pair<String, RObject>(objectAddress, unpickledFunction));

    // if post populate add elements of enclosing environment to it (object passed by ref so will
    // update in available objects list)
    if (populatePostAssignment) {
      populate_env_from_pickle_(functionEnv, functionEnvInfo, availableObjects, depth + 1);
    }

    return(unpickledFunction);
  }

  if (pickleType == "pickleEnv") {

    // get the parent environment definition
    List parentEnvInfo = pickleDefinition["parentEnv"];

    // unpickle the parent environment
    RObject parentEnv = unpickle_tree_(parentEnvInfo, availableObjects, depth + 1);

    // create env with parent as parent
    Environment pickledEnv = new_env(parentEnv);

    objectAttributes = pickleDefinition["objectAttributes"];

    // save env to list of available objects
    availableObjects.insert(std::pair<String, RObject>(objectAddress, pickledEnv));

    // get elements of environment
    List subobjectDefinitions = pickleDefinition["subObjects"];

    List subObjectDefinition, bindingFunctionDefinition;
    String subObjectType;
    String subObjectName;
    SEXP bindingFunction;

    // loop through elements
    for (List::iterator subObjectDef = subobjectDefinitions.begin();
         subObjectDef != subobjectDefinitions.end(); subObjectDef++) {

      subObjectDefinition = *subObjectDef;

      subObjectType = as<String>(subObjectDefinition["Type"]);

      // if the element is an active binding
      if (subObjectType == "pickleEnvActiveBinding") {

        // get the function definition
        bindingFunctionDefinition = subObjectDefinition["FunctionDefinition"];

        subObjectName = as<String>(bindingFunctionDefinition["objectLabel"]);

        // unpickle the function definition
        bindingFunction = unpickle_tree_(bindingFunctionDefinition, availableObjects, depth + 1);

        Symbol bindingFunctionSymbol(subObjectName);

        // make the active binding in the environment
        R_MakeActiveBinding(bindingFunctionSymbol, bindingFunction, pickledEnv);

      } else {

        // if element is a value unpickle and assign in environment

        subObjectName = as<String>(subObjectDefinition["objectLabel"]);

        pickledEnv.assign(subObjectName, unpickle_tree_(subObjectDefinition, availableObjects, depth + 1));
      }
    }

    // convert environment to RObject and apply attributes
    object = as<RObject>(pickledEnv);

    apply_object_attributes_(object, objectAttributes, availableObjects, depth + 1);

    return(object);
  }

  // stop if encounter pickleType not known
  stop("Unable to unpickle object");
}
