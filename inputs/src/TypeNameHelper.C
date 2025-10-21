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
#include "TypeNameHelper.h"
// TODO: This includes more than is actually needed but I need to go back and figure out which set
// of header files actually enables Node::Type() to be called when building the dynamic library
#include "yaml-cpp/yaml.h"

namespace inputs
{

namespace utils
{

const std::string
getNodeTypeString(const YAML::Node & node) noexcept
{
  switch (node.Type())
  {
    case YAML::NodeType::Null:
      return "Null";
    case YAML::NodeType::Scalar:
      return "Scalar";
    case YAML::NodeType::Sequence:
      return "Sequence";
    case YAML::NodeType::Map:
      return "Map";
    case YAML::NodeType::Undefined:
      return "Undefined";
    default:
      return "Unknown";
  }
}
}
}
