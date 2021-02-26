#include <Rcpp.h>
using namespace Rcpp;

String object_address_(RObject& object) {
  std::stringstream ss;
  ss << static_cast<void *>(object);
  return ss.str();
}
