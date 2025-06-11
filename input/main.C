#include "input/input.h"
#include <cstdio>
#include <boost/outcome.hpp>
namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

registerApp(Test);
registerInputBlock(Test, TestBlock);

#define LOG_ERROR(msg) identifier::log_error((msg), __FILE__, __LINE__, __PRETTY_FUNCTION__)
namespace identifier
{

// Your actual function that handles detailed debug info
void
log_error(const std::string & message, const char * file, int line, const char * function)
{
  std::cerr << file << ":" << line << ":" << function << ": " << message << "\n";
}

// Macro to automatically pass caller info
void
print_stuff(int test, std::string other)
{
  LOG_ERROR("This is a test");
  std::cout << test << " " << other << std::endl;
}
};

int
main()
{
  identifier::print_stuff(0, "this is something");
  LOG_ERROR("This is a test");
  return EXIT_SUCCESS;
}
