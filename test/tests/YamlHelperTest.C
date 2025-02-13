//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "prism/YamlHelper.h"
#include "prism/InvalidInput.h"

using namespace prism;
using namespace std;

TEST(YamlHelper, TestParamProvided)
{
  YAML::Node input;
  input["test"] = "test";

  EXPECT_TRUE(paramProvided("test", input, false));
  EXPECT_FALSE(paramProvided("test2", input, false));
  EXPECT_THROW(paramProvided("test2", input, true), InvalidInput);
}

TEST(YamlHelper, TestGetParams)
{
  YAML::Node input;
  input["test"] = "[test, tests]";

  EXPECT_THROW(getParams<double>("test", input, false), InvalidInput);
}

TEST(YamlHelper, TestGetParam)
{
  YAML::Node input;
  input["test"] = "[test, tests]";

  EXPECT_THROW(getParam<double>("test", input, false), InvalidInput);
  EXPECT_THROW(getParam<bool>("test", input, false), InvalidInput);

  EXPECT_EQ(getParam<string>("test2", input, false), "");
}

TEST(YamlHelper, TestTypeName)
{
  EXPECT_EQ(getTypeName<string>(), "std::string");
  EXPECT_EQ(getTypeName<bool>(), "bool");
  EXPECT_EQ(getTypeName<double>(), "double");
}

TEST(YamlHelper, TestDefaultValue)
{
  EXPECT_EQ(defaultValue<string>(), "");
  EXPECT_EQ(defaultValue<bool>(), false);
  EXPECT_EQ(defaultValue<double>(), 0.0);
}
