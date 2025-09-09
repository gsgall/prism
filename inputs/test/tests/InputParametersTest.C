#include "gtest/gtest.h"
#include <optional>
#include "inputs/Parameter.h"
#include "yaml-cpp/yaml.h"
#include <sstream>
#include <string>
#include "inputs/InputParameters.h"
#include "inputs/ParamMacros.h"

TEST(InputParametersTest, ValidKey)
{

  auto params = inputs::InputParameters();
  EXPECT_NO_THROW(declareRequiredParam("string-param", "a string parameter", params, std::string););

  std::istringstream input_stream("string-param: here we are");

  const std::string errors = params.parseInput(input_stream);
  EXPECT_TRUE(errors.empty()) << errors;

  EXPECT_EQ(params.getParam<std::string>("string-param"), "here we are");
}

TEST(InputParametersTest, DefaultParameter)
{

  auto params = inputs::InputParameters();
  EXPECT_NO_THROW(
      declareParam("other-param", "default", "a string parameter", params, std::string););
  EXPECT_NO_THROW(
      declareParam("string-param", "default", "a string parameter", params, std::string););

  std::istringstream input_stream("other-param: something");

  const std::string errors = params.parseInput(input_stream);
  EXPECT_TRUE(errors.empty()) << errors;

  EXPECT_EQ(params.getParam<std::string>("string-param"), "default");
  EXPECT_EQ(params.getParam<std::string>("other-param"), "something");
}

TEST(InputParametersTest, MissingRequiredParameter)
{

  auto params = inputs::InputParameters();
  EXPECT_NO_THROW(declareRequiredParam("string-param", "a string parameter", params, std::string););

  std::istringstream input_stream("other-param: something");
  const std::string errors = params.parseInput(input_stream);

  EXPECT_FALSE(errors.empty());
}
