#include "PrismErrorHelper.h"

#include <sstream>
#include <filesystem>

const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function)
{
  std::ostringstream msg;
  msg << std::filesystem::absolute(file).string() << ":" << function << ":" << line << ": "
      << message;
  return msg.str();
}
