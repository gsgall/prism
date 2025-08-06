#include "InputErrorHelper.h"

#include <sstream>
#include <filesystem>

const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function)
{
  std::ostringstream msg;
  msg << "prism::inputs Error:" << std::filesystem::absolute(file).string() << ":" << function
      << ":" << line << ": " << message;
  return msg.str();
}
