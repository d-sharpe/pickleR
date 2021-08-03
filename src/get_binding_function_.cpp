#include <Rcpp.h>
#include <Rinternals.h>
using namespace Rcpp ;

SEXP get_binding_function_ (String name, Environment& environment) {
  SEXP symbol = Rf_install( name.get_cstring());
  SEXP env = environment;
  return(R_ActiveBindingFunction(symbol, env));
}

Rboolean is_binding_function_ (String name, Environment& environment) {
  SEXP symbol = Rf_install( name.get_cstring());
  SEXP env = environment;
  return(R_BindingIsActive(symbol, env));
}

Rboolean is_environment_locked_ (Environment& environment) {
  SEXP env = environment;
  return(R_EnvironmentIsLocked(env));
}
