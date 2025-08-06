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
#include <algorithm>
#include <boost/outcome/success_failure.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
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

InputParameters::InputParameters(const InputParameters & other)
{
  for (const auto & [key, param] : other._params)
    _params[key] = param->cloneTemplate();

  for (const auto & [key, input_template] : other._block_templates)
    _block_templates[key] = input_template->cloneTemplate();
}

InputParameters &
InputParameters::operator=(const InputParameters & other)
{

  for (const auto & [key, param] : other._params)
    _params[key] = param->cloneTemplate();

  for (const auto & [key, input_template] : other._block_templates)
    _block_templates[key] = input_template->cloneTemplate();

  return *this;
}

const std::vector<std::unique_ptr<InputParameters>> &
InputParameters::blocks(const std::string & name) const noexcept(false)
{
  if (_block_templates.count(name) == 0)
    throw std::invalid_argument("No subblock with name " + name + " declared");

  return _blocks.at(name);
}

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

  std::stringstream errors;
  // the first thing that we will do is check to make sure that there are no blocks provided that
  // are no declared as valid inputs
  // this will hold the parameter name and the line on which it was provided first
  std::unordered_map<std::string, unsigned int> provided_keys;
  for (const auto & input_pair : node)
  {
    const auto input_key = input_pair.first.as<std::string>();

    if (_params.count(input_key) == 0 && _block_templates.count(input_key) == 0 &&
        _typed_block_templates.count(input_key) == 0)
    {
      std::stringstream msg;
      msg << "Undeclared parameter " << std::quoted(input_key) << " found on line "
          << input_pair.first.Mark().line + 1 << ".";
      errors << errorMessage(msg.str()) << "\n\n";
      continue;
    }

    if (const auto & [it, inserted] =
            provided_keys.insert({input_key, input_pair.first.Mark().line + 1});
        !inserted)
    {
      std::stringstream msg;
      msg << "Duplicate parameter " << std::quoted(input_key) << " provided on line " << it->second
          << " and on line " << input_pair.first.Mark().line + 1 << ".";
      errors << errorMessage(msg.str()) << "\n\n";
    }
  }
  std::cout << errors.str() << std::endl;
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

std::unique_ptr<InputParameters>
InputParameters::cloneTemplate() const noexcept
{
  auto clone = InputParameters();
  clone.addDescription(this->_description);

  for (const auto & [name, param] : _params)
    clone._params[name] = param->cloneTemplate();

  return std::make_unique<InputParameters>(std::move(clone));
}

void
InputParameters::addRepeatedBlock(const std::string & name, const InputParameters & params)
{
  // TODO: add more checking and validation here
  // we should also probably add a check for required versus not
  if (_block_templates.count(name) != 0)
    // TODO: add a better error message in this case
    throw std::invalid_argument("blag");

  _block_templates[name] = params.cloneTemplate();
}

void
InputParameters::addRepeatedTypedBlock(const std::string & name,
                                       const std::string & type,
                                       const InputParameters & params)
{
  // TODO: add more checking and validation here
  // we should also probably add a check for required versus not
  if (_typed_block_templates.count(name) != 0 && _typed_block_templates.at(name).count(type) != 0)
    // TODO: add a better error message in this case
    throw std::invalid_argument("sub block type already registered");

  _typed_block_templates.try_emplace(name);
  _typed_block_templates.at(name).emplace(type, params.cloneTemplate());
  _typed_block_templates.at(name).at(type)->addRequiredParam<std::string>(
      "type", "the type for this block");
}
}
