#include <Rcpp.h>
using namespace Rcpp;

std::string object_address_(RObject& object) {
  char buf[32];
  std::snprintf(buf, sizeof(buf), "%p", static_cast<void *>(object));
  return std::string(buf);
}
