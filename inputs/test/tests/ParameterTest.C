
#include "gtest/gtest.h"
#include <optional>
#include "inputs/Parameter.h"
#include "yaml-cpp/yaml.h"

TEST(ParameterTest, BasicGetters)
{
  auto param = std::make_unique<inputs::ParameterBase>(
      inputs::Parameter<int>("test",
                             "this parameter does something",
                             std::nullopt,
                             [](const int &) { return outcome::success(); }));

  EXPECT_EQ(param->name(), "test");
  EXPECT_EQ(param->description(), "this parameter does something");
  EXPECT_TRUE(param->required());

  param = std::make_unique<inputs::ParameterBase>(inputs::Parameter<int>(
      "test", "this parameter does something", 1, [](const int &) { return outcome::success(); }));
  EXPECT_EQ(param->name(), "test");
  EXPECT_EQ(param->description(), "this parameter does something");
  EXPECT_FALSE(param->required());

  YAML::Node node;
  node["test"] = YAML::Load("1");

  EXPECT_TRUE(param->setFromNode(node));
  auto res = param->value();
  EXPECT_TRUE(res);
  EXPECT_EQ(std::any_cast<int>(res.value()), 1);
}

TEST(ParameterTest, InvalidMap)
{
  // testing to make sure that we handle the case of duplicated keys in a map
  YAML::Node node;
  node["test"] = YAML::Load("{A: 1, A:2}");

  auto param = std::make_unique<inputs::ParameterBase>(
      inputs::Parameter<std::unordered_map<std::string, double>>(
          "test",
          "this parameter does something",
          std::nullopt,
          [](const std::unordered_map<std::string, double> &) { return outcome::success(); }));

  EXPECT_FALSE(param->setFromNode(node));
}

TEST(ParameterTest, SettingValue)
{
  auto param = std::make_unique<inputs::ParameterBase>(
      inputs::Parameter<unsigned int>("test",
                                      "this parameter does something",
                                      std::nullopt,
                                      [](const unsigned int &) { return outcome::success(); }));

  EXPECT_FALSE(param->set(int(1)));
  EXPECT_FALSE(param->set("this is a string"));
  EXPECT_FALSE(param->set(std::vector<int>({1, 2, 3})));
  EXPECT_TRUE(param->set((unsigned int)(1)));
}
