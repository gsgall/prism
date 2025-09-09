#include "InputErrorHelper.h"
#include <sstream>

const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function)
{
  std::stringstream msg;
  msg << "prism::inputs Error: " << file << ":" << function << ":" << line << ": " << message;
  return msg.str();
}
