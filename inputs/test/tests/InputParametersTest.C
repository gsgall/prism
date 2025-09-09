#include "gtest/gtest.h"
#include <filesystem>
#include <optional>
#include "inputs/Parameter.h"
#include "yaml-cpp/yaml.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "inputs/InputParameters.h"
#include "inputs/ParamMacros.h"

TEST(InputParametersTest, ValidKey)
{

  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string parameter", params, std::string););

  std::istringstream input_stream("string-param: here we are");

  const std::string errors = params.parseInput(input_stream);
  ASSERT_TRUE(errors.empty()) << errors;

  EXPECT_EQ(params.getParam<std::string>("string-param"), "here we are");
}

TEST(InputParametersTest, DefaultParameter)
{

  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(
      declareParam("other-param", "default", "a string parameter", params, std::string););
  ASSERT_NO_THROW(
      declareParam("string-param", "default", "a string parameter", params, std::string););

  std::istringstream input_stream("other-param: something");

  const std::string errors = params.parseInput(input_stream);
  ASSERT_TRUE(errors.empty()) << errors;

  EXPECT_EQ(params.getParam<std::string>("string-param"), "default");
  EXPECT_EQ(params.getParam<std::string>("other-param"), "something");
}

TEST(InputParametersTest, MissingRequiredParameter)
{

  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string parameter", params, std::string););

  std::istringstream input_stream("other-param: something");
  const std::string errors = params.parseInput(input_stream);

  ASSERT_FALSE(errors.empty());
}

TEST(InputParametersTest, BasicBlock)
{
  auto params = inputs::InputParameters();

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("double-param", -1.0, "a double", block, double));
  ASSERT_NO_THROW(declareRequiredParam("int-param", "an integer for something ", block, int));
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string", block, std::string));
  ASSERT_NO_THROW(declareRepeatedBlock("simple", block, params));

  std::istringstream input_stream(R"(
simple:
  - int-param: 1
    string-param: some string
  - int-param: 2
    double-param: 1.7
    string-param: another string
                                  )");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_TRUE(errors.empty());

  const auto & blocks = params.getBlocks("simple");

  ASSERT_EQ(blocks.size(), size_t(2));

  EXPECT_EQ(blocks[0]->getParam<int>("int-param"), 1);
  EXPECT_EQ(blocks[0]->getParam<double>("double-param"), -1.0);
  EXPECT_EQ(blocks[0]->getParam<std::string>("string-param"), "some string");

  EXPECT_EQ(blocks[1]->getParam<int>("int-param"), 2);
  EXPECT_EQ(blocks[1]->getParam<double>("double-param"), 1.7);
  EXPECT_EQ(blocks[1]->getParam<std::string>("string-param"), "another string");
}

TEST(InputParametersTest, BasicRequiredBlock)
{
  auto params = inputs::InputParameters();

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("double-param", -1.0, "a double", block, double));
  ASSERT_NO_THROW(declareRequiredParam("int-param", "an integer for something ", block, int));
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string", block, std::string));
  ASSERT_NO_THROW(declareRequiredRepeatedBlock("simple", block, params));

  std::istringstream input_stream(R"(
simple:
  - int-param: 1
    string-param: some string
  - int-param: 2
    double-param: 1.7
    string-param: another string)");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_TRUE(errors.empty());

  const auto & blocks = params.getBlocks("simple");

  ASSERT_EQ(blocks.size(), size_t(2));

  EXPECT_EQ(blocks[0]->getParam<int>("int-param"), 1);
  EXPECT_EQ(blocks[0]->getParam<double>("double-param"), -1.0);
  EXPECT_EQ(blocks[0]->getParam<std::string>("string-param"), "some string");

  EXPECT_EQ(blocks[1]->getParam<int>("int-param"), 2);
  EXPECT_EQ(blocks[1]->getParam<double>("double-param"), 1.7);
  EXPECT_EQ(blocks[1]->getParam<std::string>("string-param"), "another string");
}

TEST(InputParametersTest, MissingRequiredBlock)
{
  auto params = inputs::InputParameters();

  ASSERT_NO_THROW(declareRequiredParam("input", "something", params, std::string));

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("double-param", -1.0, "a double", block, double));
  ASSERT_NO_THROW(declareRequiredParam("int-param", "an integer for something ", block, int));
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string", block, std::string));
  ASSERT_NO_THROW(declareRequiredRepeatedBlock("simple", block, params));

  std::istringstream input_stream("input: something");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_FALSE(errors.empty());
}

TEST(InputParametersTest, InvalidBlockGet)
{
  auto params = inputs::InputParameters();

  ASSERT_NO_THROW(declareRequiredParam("input", "a string", params, std::string));

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("double-param", -1.0, "a double", block, double));
  ASSERT_NO_THROW(declareRequiredParam("int-param", "an integer for something ", block, int));
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string", block, std::string));
  ASSERT_NO_THROW(declareRepeatedBlock("simple", block, params));

  std::istringstream input_stream("input: something");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_TRUE(errors.empty());

  EXPECT_THROW(static_cast<void>(params.getBlocks("simple")), std::invalid_argument);
  EXPECT_THROW(static_cast<void>(params.getBlocks("undeclared-block")), std::invalid_argument);
}

TEST(InputParametersTest, UserProvidedDuplicateKeys)
{
  // testing to make sure that we handle the case of duplicated keys in a map
  auto params = inputs::InputParameters();

  ASSERT_NO_THROW(
      (declareRequiredParam("map", "a map", params, std::unordered_map<std::string, int>)));

  std::istringstream input_stream("map: {A: 1, A:2}");

  const std::string errors = params.parseInput(input_stream);
  ASSERT_FALSE(errors.empty());
}

TEST(InputParametersTest, CollidingParameterNames)
{
  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(declareParam("param", "value", "just a regular param", params, std::string));
  EXPECT_THROW(declareParam("param", -1.0, "a double", params, double), std::invalid_argument);

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("param", "value", "just a regular param", block, std::string));
  EXPECT_THROW(declareRepeatedBlock("param", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedBlock("param", block, params), std::invalid_argument);

  EXPECT_THROW(declareRepeatedTypedBlock("param", "type", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedTypedBlock("param", "type", block, params),
               std::invalid_argument);

  ASSERT_NO_THROW(declareRequiredRepeatedBlock("block", block, params));
  EXPECT_THROW(declareRepeatedBlock("block", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedBlock("block", block, params), std::invalid_argument);
  EXPECT_THROW(declareRepeatedTypedBlock("block", "type", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedTypedBlock("block", "type", block, params),
               std::invalid_argument);
  EXPECT_THROW(declareParam("block", -1, "an int", params, int), std::invalid_argument);

  ASSERT_NO_THROW(declareRequiredRepeatedTypedBlock("block2", "type", block, params));
  EXPECT_THROW(declareRepeatedBlock("block2", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedBlock("block2", block, params), std::invalid_argument);
  EXPECT_THROW(declareRepeatedTypedBlock("block2", "type", block, params), std::invalid_argument);
  EXPECT_THROW(declareRequiredRepeatedTypedBlock("block2", "type", block, params),
               std::invalid_argument);

  EXPECT_THROW(declareParam("block2", -1, "an int", params, int), std::invalid_argument);
}
