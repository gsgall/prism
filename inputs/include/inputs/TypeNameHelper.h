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

namespace YAML
{
class Node;
}

namespace inputs
{
namespace utils
{
template <typename T>
std::string typeName();

const std::string getNodeTypeString(const YAML::Node & node) noexcept;

}
}
