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
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace YAML
{
class Node;
}

namespace inputs
{
namespace utils
{
template <typename T>
std::string
typeName()
{
  if constexpr (std::is_same_v<T, int>)
  {
    return "int";
  }
  else if constexpr (std::is_same_v<T, bool>)
  {
    return "bool";
  }
  else if constexpr (std::is_same_v<T, double>)
  {
    return "double";
  }
  if constexpr (std::is_same_v<T, unsigned int>)
  {
    return "unsigned int";
  }
  else if constexpr (std::is_same_v<T, std::string>)
  {
    return "std::string";
  }
  else if constexpr (std::is_same_v<T, std::vector<int>>)
  {
    return "std::vector<int>";
  }
  else if constexpr (std::is_same_v<T, std::vector<double>>)
  {
    return "std::vector<double>";
  }
  else if constexpr (std::is_same_v<T, std::vector<unsigned int>>)
  {
    return "std::vector<unsigned int>";
  }
  else if constexpr (std::is_same_v<T, std::vector<std::string>>)
  {
    return "std::vector<std::string>";
  }
  else if constexpr (std::is_same_v<T, std::unordered_map<std::string, int>>)
  {
    return "std::unordered_map<std::string, int>";
  }
  else if constexpr (std::is_same_v<T, std::unordered_map<std::string, double>>)
  {
    return "std::unordered_map<std::string, double>";
  }
  else if constexpr (std::is_same_v<T, std::unordered_map<std::string, std::string>>)
  {
    return "std::unordered_map<std::string, std::string>";
  }
  else if constexpr (std::is_same_v<T, std::unordered_map<std::string, unsigned int>>)
  {
    return "std::unordered_map<std::string, unsigned int>";
  }
}

const std::string getNodeTypeString(const YAML::Node & node) noexcept;

}
}
