#include <Rcpp.h>
#include "pickle_funs.hpp"
using namespace Rcpp;


List pickle_tree_(RObject& object,
                  String objectLabel,
                  std::unordered_map<String, RObject>& seenObjects,
                  std::unordered_set<String>& seenAddresses,
                  int depth = 1) {

  RObject objectNoAttributes;

  // grab object address
  String objectAddress = object_address_(object);

  // switch on type of SEXP object passed to function
  switch(TYPEOF(object)) {
  // if object is R NULL
  case NILSXP:
    return(List::create(_["objectLabel"] = objectLabel,
                        _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                        _["Type"] = "pickleNULL"));
    break;

  // if object is base vector type
  case LGLSXP:
    case INTSXP:
      case REALSXP:
        case CPLXSXP:
          case RAWSXP:
            case CHARSXP:
              case STRSXP:

                // copy the object and strip all attributes from it for saving
                // in the unique object map
                // attributes will be saved as "pickleAttributes" on pickleDefiniton
                // for reapplication during unpickle
                objectNoAttributes = strip_object_attributes_(object);

                // try insert object via address into unique object map
                if (!seenObjects.insert(std::pair<String, RObject>(objectAddress, objectNoAttributes)).second) {
                  // return a pickleReference to existing object in map
                  return(List::create(_["objectLabel"] = objectLabel,
                                      _["objectAddress"] = objectAddress,
                                      _["Type"] = "pickleReference"));
                } else {
                  // return a pickle indicating the object details
                  return(List::create(_["objectLabel"] = objectLabel,
                                      _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                                      _["objectAddress"] = objectAddress,
                                      _["Type"] = "pickle"));
                }

                break;

  // if object is a R list type
  case VECSXP:
    // try insert address of object into set of observed addresses
    // if already seen return a pickle reference and skip recursive
    // deconstruction of object
    if (!seenAddresses.insert(objectAddress).second) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["Type"] = "pickleReference"));
    } else {
      //if first time seeing object address - deconstruct

      // get list length
      int listObjectLength = XLENGTH(object);

      // allocate list for element pickle results
      List pickledSubObjects (listObjectLength);

      // allocate vector for list names
      CharacterVector listObjectNames(listObjectLength);

      // if list has names retrieve
      if (object.hasAttribute("names")) {
        listObjectNames = wrap(object.attr("names"));
      }

      // loop over each list element and recursively pickle
      for (R_xlen_t i = 0; i < listObjectLength; i++) {

        RObject listElement = VECTOR_ELT(object, i);

        pickledSubObjects[i] = pickle_tree_(listElement, listObjectNames[i], seenObjects, seenAddresses, depth + 1);
      }

      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                          _["subObjects"] = pickledSubObjects,
                          _["Type"] = "pickleList"));

    }
    break;
  case SYMSXP:
    case EXPRSXP:
      case LANGSXP:
        // copy the object and strip all attributes from it for saving
        // in the unique object map
        // attributes will be saved as "pickleAttributes" on pickleDefiniton
        // for reapplication during unpickle
        objectNoAttributes = strip_object_attributes_(object);

        // try insert object via address into unique object map
        if (!seenObjects.insert(std::pair<String, RObject>(objectAddress, objectNoAttributes)).second) {
          // return a pickleReference to existing object in map
          return(List::create(_["objectLabel"] = objectLabel,
                              _["objectAddress"] = objectAddress,
                              _["Type"] = "pickleReference"));
        } else {
          // return a pickle indicating the object details
          return(List::create(_["objectLabel"] = objectLabel,
                              _["objectAddress"] = objectAddress,
                              _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                              _["Type"] = "pickle"));
        }

        break;
  case ENVSXP:

    // if object is one of the built-in R environments save a reference to that
    // environment
    if (object == R_BaseEnv) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["Type"] = "pickleBaseEnv"));
    }

    if (object == R_GlobalEnv) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["Type"] = "pickleGlobalEnv"));
    }

    if (object == R_EmptyEnv) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["Type"] = "pickleEmptyEnv"));
    }

    // if the object is a package or namespace environment save a reference to
    // that environment
    if (R_IsPackageEnv(object)) {
      String package_name = String(R_PackageEnvName(object));

      return(List::create(_["objectLabel"] = objectLabel,
                          _["Namespace"] = package_name,
                          _["Type"] = "picklePackageEnv"));
    }

    if (R_IsNamespaceEnv(object)) {
      String package_name = String(R_NamespaceEnvSpec(object));

      return(List::create(_["objectLabel"] = objectLabel,
                          _["Namespace"] = package_name,
                          _["Type"] = "pickleNamespaceEnv"));
    }

    // try insert address of object into set of observed addresses
    // if already seen return a pickle reference and skip recursive
    // deconstruction of object
    if (!seenAddresses.insert(objectAddress).second) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["Type"] = "pickleReference"));
    } else {

      // convert object to environment
      Environment objectAsEnvironment = as<Environment>(object);

      // get parent environment of the current environment object
      RObject parentEnvironment = as<RObject>(objectAsEnvironment.parent());

      // pickle the parent environment
      List parentEnvironmentPickleDefinition = pickle_tree_(parentEnvironment, "", seenObjects, seenAddresses, depth + 1);

      // get the names of all (inc elements starting with ".") elements in environment
      CharacterVector objectAsEnvElementNames = objectAsEnvironment.ls(true);

      // get number of elements in the environment
      int numberOfEnvironmentElements = objectAsEnvElementNames.length();

      // allocate list for element pickle results
      List pickledSubObjects (numberOfEnvironmentElements);

      // allocate element name, element value, and a activebinding pickle definition list for loop
      String elementName;
      RObject environmentElement;
      List pickledActiveBindingEnvironmentElement;

      for (int i = 0; i < numberOfEnvironmentElements; i++) {

        elementName = objectAsEnvElementNames[i];

        // if the element is not an active binding pickle the value
        if (!is_binding_function_(elementName, objectAsEnvironment)) {

          environmentElement = objectAsEnvironment.get(elementName);

          pickledSubObjects[i] = pickle_tree_(environmentElement, elementName, seenObjects, seenAddresses, depth + 1);
        } else {

          // if the element is an active binding get the bound function definition and pickle that

          environmentElement = get_binding_function_(elementName, objectAsEnvironment);

          pickledActiveBindingEnvironmentElement = pickle_tree_(environmentElement, elementName, seenObjects, seenAddresses, depth + 1);

          pickledSubObjects[i] = List::create(_["FunctionDefinition"] = pickledActiveBindingEnvironmentElement,
                                              _["Type"] = "pickleEnvActiveBinding");
        }
      }

      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                          _["parentEnv"] = parentEnvironmentPickleDefinition,
                          _["subObjects"] = pickledSubObjects,
                          _["Type"] = "pickleEnv"));
    }
    break;
  case CLOSXP:
    // try insert address of object into set of observed addresses
    // if already seen return a pickle reference and skip recursive
    // deconstruction of object
    if (!seenAddresses.insert(objectAddress).second) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["Type"] = "pickleReference"));
    } else {

      // get the enclosing environment, formal arguments and body of the function
      RObject objectEnclosingEnvironment = CLOENV(object);
      RObject objectFormals = FORMALS(object);
      RObject objectBody = BODY(object);

      // pickle the elements that compose the function and return
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                          _["objectEnvironment"] = pickle_tree_(objectEnclosingEnvironment, "", seenObjects, seenAddresses, depth + 1),
                          _["objectFormals"] = pickle_tree_(objectFormals, "", seenObjects, seenAddresses,  depth + 1),
                          _["objectBody"] = pickle_tree_(objectBody, "", seenObjects, seenAddresses,  depth + 1),
                          _["Type"] = "pickleFunction"));
    }

    break;
  case LISTSXP:
    // copy the object and strip all attributes from it for saving
    // in the unique object map
    // attributes will be saved as "pickleAttributes" on pickleDefiniton
    // for reapplication during unpickle
    objectNoAttributes = strip_object_attributes_(object);

    // try insert object via address into unique object map
    if (!seenObjects.insert(std::pair<String, RObject>(objectAddress, objectNoAttributes)).second) {
      // return a pickleReference to existing object in map
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["Type"] = "pickleReference"));
    } else {
      // return a pickle indicating the object details
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                          _["Type"] = "pickle"));
    }
    break;
  case BCODESXP:

    // get the byte code constants
    List bytecodeConsts = BCODE_CONSTS(object);

    // first element is the language object i.e. body of function
    RObject byteCodeLanguageComponent = bytecodeConsts[0];

    // strip object off all attributes and save as copy
    objectNoAttributes = strip_object_attributes_(byteCodeLanguageComponent);

    // try insert address of object into set of observed addresses
    // if already seen return a pickle reference and skip recursive
    // deconstruction of object
    if (!seenObjects.insert(std::pair<String, RObject>(objectAddress, objectNoAttributes)).second) {
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["Type"] = "pickleReference"));
    } else {
      // return a pickle indicating the object details
      return(List::create(_["objectLabel"] = objectLabel,
                          _["objectAddress"] = objectAddress,
                          _["objectAttributes"] = extract_object_attributes_(object, seenObjects, seenAddresses),
                          _["Type"] = "pickle"));
    }
  }

  stop("Unable to pickle");
}
