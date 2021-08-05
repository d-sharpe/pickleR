#include <Rcpp.h>
#include <Rinternals.h>
using namespace Rcpp ;

SEXP get_binding_function_ (std::string name, Environment& environment) {
  SEXP symbol = Rf_install(name.c_str());
  SEXP env = environment;
  return(R_ActiveBindingFunction(symbol, env));
}

Rboolean is_binding_function_ (std::string name, Environment& environment) {
  SEXP symbol = Rf_install( name.c_str());
  SEXP env = environment;
  return(R_BindingIsActive(symbol, env));
}

Rboolean is_environment_locked_ (Environment& environment) {
  SEXP env = environment;
  return(R_EnvironmentIsLocked(env));
}
