#include "PrismErrorHelper.h"
#include <sstream>

const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function)
{
  std::ostringstream msg;
  msg << "prism Error: " << file << ":" << function << ":" << line << ": " << message;
  return msg.str();
}
