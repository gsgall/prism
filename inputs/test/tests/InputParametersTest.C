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
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string parameter", params, std::string));

  std::istringstream input_stream("string-param: here we are");

  const std::string errors = params.parseInput(input_stream);
  ASSERT_TRUE(errors.empty()) << errors;

  EXPECT_EQ(params.getParam<std::string>("string-param"), "here we are");
}

TEST(InputParametersTest, DefaultParameter)
{

  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(
      declareParam("other-param", "default", "a string parameter", params, std::string));
  ASSERT_NO_THROW(
      declareParam("string-param", "default", "a string parameter", params, std::string));

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

TEST(InputParametersTest, BasicRequiredTypedBlock)
{
  auto params = inputs::InputParameters();

  auto block = inputs::InputParameters();

  ASSERT_NO_THROW(declareParam("double-param", -1.0, "a double", block, double));
  ASSERT_NO_THROW(declareRequiredParam("int-param", "an integer for something ", block, int));
  ASSERT_NO_THROW(declareRequiredParam("string-param", "a string", block, std::string));
  ASSERT_NO_THROW(declareRequiredRepeatedTypedBlock("simple", "version1", block, params));

  std::istringstream input_stream(R"(
simple:
  - int-param: 1
    type: version1
    string-param: some string
  - int-param: 2
    type: version1
    double-param: 1.7
    string-param: another string)");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_TRUE(errors.empty());

  const auto & blocks = params.getTypedBlocks("simple", "version1");

  ASSERT_EQ(blocks.size(), size_t(2));

  EXPECT_EQ(blocks[0]->getParam<int>("int-param"), 1);
  EXPECT_EQ(blocks[0]->getParam<double>("double-param"), -1.0);
  EXPECT_EQ(blocks[0]->getParam<std::string>("string-param"), "some string");

  EXPECT_EQ(blocks[1]->getParam<int>("int-param"), 2);
  EXPECT_EQ(blocks[1]->getParam<double>("double-param"), 1.7);
  EXPECT_EQ(blocks[1]->getParam<std::string>("string-param"), "another string");
}

TEST(InputParametersTest, MultipleRequiredTypedBlock)
{
  auto params = inputs::InputParameters();

  auto block_base = inputs::InputParameters();

  ASSERT_NO_THROW(declareRequiredParam(
      "common", "a parameter both need", block_base, std::vector<std::string>));

  auto block_type1 = block_base;

  ASSERT_NO_THROW(declareRequiredParam("param-one", "a param for block type 1", block_type1, int));

  auto block_type2 = block_base;

  ASSERT_NO_THROW(
      declareRequiredParam("param-two", "a param for block type 2", block_type2, std::string));

  ASSERT_NO_THROW(declareRepeatedTypedBlock("simple", "one", block_type1, params));
  ASSERT_NO_THROW(declareRepeatedTypedBlock("simple", "two", block_type2, params));

  std::istringstream input_stream(R"(
simple:
  - type: one
    param-one: 1
    common: [a string]
  - type: two
    param-two: a param
    common: [a different string, and another])");

  const std::string errors = params.parseInput(input_stream);

  ASSERT_TRUE(errors.empty()) << errors;

  const auto & type_1_blocks = params.getTypedBlocks("simple", "one");
  const auto & type_2_blocks = params.getTypedBlocks("simple", "two");

  ASSERT_EQ(type_1_blocks.size(), size_t(1));
  ASSERT_EQ(type_2_blocks.size(), size_t(1));

  EXPECT_EQ(type_1_blocks[0]->getParam<int>("param-one"), 1);
  EXPECT_EQ(type_1_blocks[0]->getParam<std::vector<std::string>>("common"),
            std::vector<std::string>({"a string"}));
  EXPECT_EQ(type_2_blocks[0]->getParam<std::string>("param-two"), "a param");
  EXPECT_EQ(type_2_blocks[0]->getParam<std::vector<std::string>>("common"),
            std::vector<std::string>({"a different string", "and another"}));
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

  EXPECT_TRUE(params.getBlocks("simple").empty());
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

TEST(InputParametersTest, SetParametersWithParsing)
{
  // testing to make sure that we handle the case of duplicated keys in a map
  auto params = inputs::InputParameters();

  ASSERT_NO_THROW((declareParam("map", {}, "a map", params, std::unordered_map<std::string, int>)));
  ASSERT_NO_THROW((declareRequiredParam("param", "a string param", params, std::string)));

  std::istringstream input_stream("param: something");

  const std::string errors = params.parseInput(input_stream);
  ASSERT_TRUE(errors.empty());

  const auto map = std::unordered_map<std::string, int>({{"A", 1}, {"B", 2}});
  ASSERT_NO_THROW((params.setParam<std::unordered_map<std::string, int>>("map", map)));
  EXPECT_THROW(params.setParam<int>("map", 1), std::invalid_argument);
  EXPECT_THROW(params.setParam<int>("undefined-param", 1), std::invalid_argument);
  EXPECT_EQ((params.getParam<std::unordered_map<std::string, int>>("map")), map);
  EXPECT_EQ((params.getParam<std::string>("param")), "something");
}

TEST(InputParametersTest, SetParametersNoParsing)
{
  // testing to make sure that we handle the case of duplicated keys in a map
  auto params = inputs::InputParameters();

  ASSERT_NO_THROW((declareParam("map", {}, "a map", params, std::unordered_map<std::string, int>)));
  ASSERT_NO_THROW((declareRequiredParam("param", "a string param", params, std::string)));

  const auto map = std::unordered_map<std::string, int>({{"A", 1}, {"B", 2}});
  ASSERT_NO_THROW((params.setParam<std::unordered_map<std::string, int>>("map", map)));
  ASSERT_NO_THROW((params.setParam<std::string>("param", "something")));
  EXPECT_THROW(params.setParam<int>("map", 1), std::invalid_argument);
  EXPECT_THROW(params.setParam<int>("undefined-param", 1), std::invalid_argument);
  EXPECT_EQ((params.getParam<std::unordered_map<std::string, int>>("map")), map);
  EXPECT_EQ((params.getParam<std::string>("param")), "something");
}

struct SpeciesData
{
  /// the unique id for the species (guaranteed to be in the range 0-(n-1) where n in the number unique species)
  unsigned int id;
  /// the number of times the species occurs on a side of the reaction
  unsigned int occurances;
};

namespace YAML
{
template <>
struct convert<SpeciesData>
{
  static Node encode(const SpeciesData & rhs)
  {
    Node node;
    node["id"] = rhs.id;
    node["occurances"] = rhs.occurances;
    return node;
  }

  static bool decode(const Node & node, SpeciesData & data)
  {
    if (!node.IsMap())
      return false;
    if (!node["id"].IsDefined() || !node["occurances"].IsDefined())
      return false;

    data.id = node["id"].as<unsigned int>();
    data.occurances = node["occurances"].as<unsigned int>();
    return true;
  }
};
}
namespace inputs
{
namespace utils
{
template <>
std::string
typeName<SpeciesData>()
{
  return "SpeciesData";
}
}
}

TEST(InputParametersTest, CustomStructTest)
{

  auto params = inputs::InputParameters();
  ASSERT_NO_THROW(declareRequiredParam(
      "species-data", "A custom struct needed for input", params, SpeciesData));

  SpeciesData testData{0, 2};
  ASSERT_NO_THROW((params.setParam<SpeciesData>("species-data", testData)));

  const auto data = params.getParam<SpeciesData>("species-data");

  EXPECT_EQ(data.id, static_cast<unsigned int>(0));
  EXPECT_EQ(data.occurances, static_cast<unsigned int>(2));
}
