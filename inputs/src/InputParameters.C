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
#include "InputParameters.h"
#include "InputErrorHelper.h"
#include <boost/outcome/success_failure.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <yaml-cpp/emittermanip.h>
#include "TypeNameHelper.h"

///@{
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/emit.h"
///@}
namespace inputs
{

InputParameters::InputParameters() {};

void
InputParameters::addDescription(const std::string & description) noexcept
{
  _description = description;
}

const std::string &
InputParameters::description() const noexcept
{
  return _description;
}

void
InputParameters::readFromNodes(const YAML::Node & node) noexcept(false)
{

  if (!node.IsMap())
  {
    std::stringstream msg;
    msg << "The provided node must be a map and the provided node was a "
        << utils::getNodeTypeString(node) << "\n\nContents\n"
        << node;
    throw std::invalid_argument("\n" + errorMessage(msg.str()));
  }

  std::stringstream failures;
  // the first thing we are going to be doing is to check that there are no parameters that have not
  // been declared in the input
  // we should also check to make sure that there are no parameters that have been supplied twice
  std::unordered_map<std::string, int> provided_params;
  for (const auto & input_pairs : node)
  {
    bool found = false;
    for (const auto & param_pairs : _params)
      if (input_pairs.first.as<std::string>() == param_pairs.first)
      {
        found = true;
        break;
      }
    // if the parameter is not allowed then we report it and move onto the next one
    if (!found)
    {
      std::stringstream msg;
      msg << "Extra parameter " << std::quoted(input_pairs.first.as<std::string>())
          << " found on line " << input_pairs.first.Mark().line + 1 << " with contents: \""
          << input_pairs.first << ": " << input_pairs.second << "\"";
      failures << std::endl << errorMessage(msg.str());
      continue;
    }
    // now we can check to see if there are any nodes that have been provided more than once in the
    // same block
    if (provided_params.count(input_pairs.first.as<std::string>()) != 0)
    {
      const std::string & param_name = input_pairs.first.as<std::string>();
      std::stringstream msg;
      msg << "Parameter " << std::quoted(param_name)
          << " provided multiple times. Parameter found on line " << provided_params[param_name]
          << " and on line " << input_pairs.first.Mark().line + 1 << ".";
      failures << std::endl << errorMessage(msg.str());
      continue;
    }

    provided_params[input_pairs.first.as<std::string>()] = input_pairs.first.Mark().line + 1;
  }

  if (!failures.str().empty())
    failures << std::endl;

  for (auto & [keys, param] : _params)
  {
    if (const auto res = param->setFromNode(node); !res)
      failures << "\n" << res.error();
  }

  if (!failures.str().empty())
  {
    std::stringstream msg;
    msg << "\n\nInput errors found in node starting at line " << node.Mark().line + 1 << "\n\n"
        << node << "\n"
        << failures.str();
    throw std::invalid_argument(msg.str());
  }
}

void
InputParameters::duplicateParamChecker(const std::string & name) const noexcept(false)
{

  if (_params.count(name) != 0)
  {
    std::stringstream msg, msg2;

    if (!_params.at(name)->file().empty() || _params.at(name)->lineNumber() != -1)
    {
      msg << "\n"
          << errorWithContext("Previous parameter decleration location.",
                              _params.at(name)->file().c_str(),
                              _params.at(name)->lineNumber(),
                              _params.at(name)->function().c_str());
    }

    msg2 << "Param with name " << std::quoted(name) << " and type "
         << std::quoted(_params.at(name)->typeName()) << " already exists.";
    msg << "\n" << errorWithContext(msg2.str(), __FILE__, __LINE__, __FUNCTION__);

    throw std::invalid_argument(msg.str());
  }
}

}
