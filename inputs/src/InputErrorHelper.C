#include "InputErrorHelper.h"
#include <sstream>

namespace inputs
{
const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function)
{
  std::stringstream msg;
  msg << "input Error: " << file << ":" << function << ":" << line << ": " << message;
  return msg.str();
}
}
