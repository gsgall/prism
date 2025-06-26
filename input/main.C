#include "input/input.h"
#include "yaml-cpp/yaml.h"
#include <boost/outcome.hpp>
#include <memory>
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
  auto p = input::Parameter<std::vector<std::string>>("test", "this is a test parameter");
  auto p2 = input::Parameter<double>("test2", "this is a test parameter");

  std::vector<std::unique_ptr<input::ParameterBase>> test = {
      std::make_unique<input::Parameter<unsigned int>>("test", "test")};

  for (const auto & i : test)
    std::cout << dynamic_cast<type>() YAML::Node input;

  // input["test"] = "[first, second, third]";
  //  input["test"] = YAML::Load("[first, second, third]");
  // input["test"] = YAML::Load("first");
  input["test"] = "test";
  //  input["test2"] = "2.0";
  //  if (p.value())
  //    std::cout << p.value().value() << std::endl;
  //  else
  //    std::cout << "No value" << std::endl;
  //
  if (const auto & result = p.parseInput(input))
  {
    if (const auto & vec_opt = p.value(); vec_opt.has_value())
    {
      for (const auto & i : vec_opt.value())
      {
        std::cout << i << " ";
      }
      std::cout << std::endl;
    }
  }
  else
    std::cout << result.error() << std::endl;

  if (const auto & result = p2.parseInput(input))
  {
    std::cout << p2.value().value() << std::endl;
  }
  else
  {
    std::cout << result.error() << std::endl;
  }
  //    if (const auto & vec_opt = p.value(); vec_opt.has_value())
  //    {
  //      for (const auto & i : vec_opt.value())
  //      {
  //        std::cout << i << " ";
  //      }
  //      std::cout << std::endl;
  //    }
  return EXIT_SUCCESS;
}
