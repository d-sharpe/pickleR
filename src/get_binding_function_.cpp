#include <Rcpp.h>
#include <Rinternals.h>
using namespace Rcpp ;

#define HASHSIZE(x)     LENGTH(x)
#define HASHVALUE(x)    TRUELENGTH(x)

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
