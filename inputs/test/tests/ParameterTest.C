
#include "gtest/gtest.h"
#include <optional>
#include "inputs/Parameter.h"
#include "yaml-cpp/yaml.h"

TEST(ParameterTest, BasicGetters)
{
  std::unique_ptr<inputs::ParameterBase> param =
      std::make_unique<inputs::Parameter<int>>("test",
                                               std::nullopt,
                                               "this parameter does something",
                                               [](const int &) { return outcome::success(); });

  EXPECT_EQ(param->name(), "test");
  EXPECT_EQ(param->description(), "this parameter does something");
  EXPECT_TRUE(param->required());

  param = std::make_unique<inputs::Parameter<int>>(
      "test", 1, "this parameter does something", [](const int &) { return outcome::success(); });

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

  std::unique_ptr<inputs::ParameterBase> param =
      std::make_unique<inputs::Parameter<std::unordered_map<std::string, double>>>(
          "test",
          std::nullopt,
          "this parameter does something",
          [](const std::unordered_map<std::string, double> &) { return outcome::success(); });

  EXPECT_FALSE(param->setFromNode(node));
}

TEST(ParameterTest, SettingValue)
{
  std::unique_ptr<inputs::ParameterBase> param = std::make_unique<inputs::Parameter<unsigned int>>(
      "test",
      std::nullopt,
      "this parameter does something",
      [](const unsigned int &) { return outcome::success(); });

  EXPECT_FALSE(param->set(int(1)));
  EXPECT_FALSE(param->set("this is a string"));
  EXPECT_FALSE(param->set(std::vector<int>({1, 2, 3})));
  EXPECT_TRUE(param->set((unsigned int)(1)));

  const auto res = param->value();

  EXPECT_TRUE(res) << res.error();
  //  EXPECT_EQ(std::any_cast<unsigned int>(res.value()), (unsigned int)(1));
}
