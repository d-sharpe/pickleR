#include <Rcpp.h>
using namespace Rcpp;

std::string object_address_(RObject& object) {
  std::stringstream ss;
  ss << static_cast<void *>(object);
  return ss.str();
}
