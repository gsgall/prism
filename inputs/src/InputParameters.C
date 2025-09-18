//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//* * Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "InputParameters.h"
#include "InputErrorHelper.h"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include "TypeNameHelper.h"

///@{
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/emit.h"
#include <yaml-cpp/node/parse.h>
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

  _required_blocks = other._required_blocks;
}

InputParameters &
InputParameters::operator=(const InputParameters & other)
{

  for (const auto & [key, param] : other._params)
    _params[key] = param->cloneTemplate();

  for (const auto & [key, input_template] : other._block_templates)
    _block_templates[key] = input_template->cloneTemplate();

  _required_blocks = other._required_blocks;
  return *this;
}

const std::vector<std::unique_ptr<InputParameters>> &
InputParameters::getBlocks(const std::string & name) const noexcept(false)
{
  if (_block_templates.count(name) == 0)
    throw std::invalid_argument("No block with name " + name + " declared.");

  return _blocks.at(name);
}

const std::vector<std::unique_ptr<InputParameters>> &
InputParameters::getTypedBlocks(const std::string & name, const std::string & type) const
    noexcept(false)
{
  if (_typed_block_templates.count(name) == 0 || _typed_block_templates.at(name).count(type) == 0)
    throw std::invalid_argument("No typed block with name \"" + name + "\" and type \"" + type +
                                "\" declared.");

  return _typed_blocks.at(name).at(type);
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

const std::string
InputParameters::parseInput(const std::string & filepath) noexcept
{
  if (std::ifstream file(filepath); !file)
  {
    return errorMessage("Unable to open file \"" + filepath + "\"");
  }
  else
    return readFromNodes(YAML::Load(file), filepath, "");
}

const std::string
InputParameters::parseInput(std::istream & stream) noexcept
{
  if (!stream)
  {
    return errorMessage("Bad stream provided");
  }
  else
    return readFromNodes(YAML::Load(stream), "input stream", "");
}

const std::pair<std::unordered_map<std::string, unsigned int>, std::string>
InputParameters::invalidKeyAndDuplicateCheck(const YAML::Node & nodes) noexcept
{
  std::stringstream errors;
  std::unordered_map<std::string, unsigned int> provided_keys;
  for (const std::pair<YAML::Node, YAML::Node> & input_pair : nodes)
  {
    const auto & input_key = input_pair.first.as<std::string>();

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

  return std::make_pair(provided_keys, errors.str());
}

const std::string
InputParameters::checkForRequiredParamsAndBlocks(
    const YAML::Node & nodes, std::unordered_map<std::string, unsigned int> provided_keys) noexcept
{
  std::stringstream errors;
  for (const auto & [name, param] : _params)
  {
    if (provided_keys.count(name) == 1 || !param->required())
      continue;

    std::stringstream msg;
    msg << "Required parameter " << std::quoted(name) << " of type "
        << std::quoted(param->typeName()) << " was not provided\n";
    errors << errorMessage(msg.str());

    if (!param->file().empty() && !param->function().empty() && param->lineNumber() != -1)
      errors << errorWithContext("Required parameter \"" + name + "\" declaration location.\n",
                                 param->file().c_str(),
                                 param->lineNumber(),
                                 param->function().c_str());
  }

  for (const auto & name : _required_blocks)
  {
    if (provided_keys.count(name) == 0)
    {
      std::stringstream msg;
      msg << "Required block " << std::quoted(name) << " was not provided\n";
      errors << errorMessage(msg.str());
      return errors.str();
    }

    if (!nodes[name].IsSequence())
    {
      std::stringstream msg;
      msg << "Required block " << std::quoted(name) << " provided but was not a sequence\n";
      errors << errorMessage(msg.str());
      continue;
    }
  }

  for (const auto & [name, types] : _required_typed_blocks)
  {
    if (provided_keys.count(name) == 0)
    {
      std::stringstream msg;
      msg << "Required block " << std::quoted(name) << " was not provided\n";
      errors << errorMessage(msg.str());
      continue;
    }
    if (!nodes[name].IsSequence())
    {
      std::stringstream msg;
      msg << "Required block " << std::quoted(name) << " provided but was not a sequence\n";
      errors << errorMessage(msg.str());
      continue;
    }
    for (const std::string & type : types)
    {
      bool found = false;
      for (const YAML::Node & block : nodes[name])
      {
        if (!block["type"].IsDefined())
        {
          std::stringstream msg;
          msg << "Block " << std::quoted(name)
              << " was declared as a typed block but contains a subblock without the "
              << std::quoted("type") << " key word.\n";
          errors << errorMessage(msg.str());
          continue;
        }

        if (types.count(block["type"].as<std::string>()) == 0)
        {
          std::stringstream msg;
          msg << "Block " << std::quoted(name + "/" + type) << "was provided but "
              << std::quoted(type) << " is not a registered type.\n";
          errors << errorMessage(msg.str());
          continue;
        }
        if (block["type"].as<std::string>() == type)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        std::stringstream msg;
        msg << "Parent block " << std::quoted(name)
            << " provided, but required subblock of type: " << std::quoted(type) << " was not.\n";
        errors << errorMessage(msg.str());
      }
    }
  }
  return errors.str();
}

const std::string
InputParameters::parseBlocks(const YAML::Node & node, const std::string & block_name) noexcept
{
  std::stringstream errors;

  // we are not reporting an error about required blocks here since another functions checks that
  // required blocks are provided and this would just add the same message twice so if the block is
  // not in the input we'll just do nothing
  if (!node[block_name].IsDefined())
  {
    return "";
  }

  for (const YAML::Node input_block : node[block_name])
  {
    auto & block =
        _blocks.at(block_name).emplace_back(_block_templates.at(block_name)->cloneTemplate());
    errors << block->readFromNodes(input_block, "", block_name);
  }
  return errors.str();
}

const std::string
InputParameters::parseTypedBlocks(const YAML::Node & node,
                                  const std::string & block_name,
                                  const std::string & block_type) noexcept
{
  std::stringstream errors;

  // similarly to parseBlocks since other functions check for required blocks we will not do that
  // here instead we'll just skip if the requested block does not exist
  for (const YAML::Node input_block : node[block_name])
  {
    if (!input_block["type"].IsDefined())
      continue;
    if (input_block["type"].as<std::string>() != block_type)
      continue;

    auto & block =
        _typed_blocks.at(block_name)
            .at(block_type)
            .emplace_back(_typed_block_templates.at(block_name).at(block_type)->cloneTemplate());
    errors << block->readFromNodes(input_block, "", block_name + "/" + block_type);
  }

  return errors.str();
}
const std::string
InputParameters::readFromNodes(const YAML::Node & node,
                               const std::string & filepath,
                               const std::string & block_name) noexcept
{
  std::stringstream errors;
  if (!node.IsMap())
  {
    std::stringstream msg;
    msg << "The provided node must be a map and the provided node was a "
        << utils::getNodeTypeString(node) << "\n\nContents\n"
        << node;
    errors << errorMessage(msg.str());
    return errors.str();
  }
  const auto [provided_keys, invalid_errors] = invalidKeyAndDuplicateCheck(node);
  if (!invalid_errors.empty() && !block_name.empty())
    errors << errorMessage("Invalid param detected when parsing block of type " + block_name +
                           "\n");
  errors << invalid_errors;
  errors << checkForRequiredParamsAndBlocks(node, provided_keys);

  for (auto & [name, param] : _params)
  {
    // if the node was not provided and it's not required that's fine we'll return the default value
    // when they ask for it
    if (!node[name].IsDefined() && !param->required())
      continue;
    // if the parameter is required but it was not provided then we will also skip this since the
    // error will have been reported by the requiredParams check
    if (!node[name].IsDefined() && param->required())
      continue;

    const auto res = param->setFromNode(node);
    if (res)
      continue;

    std::stringstream msg;
    msg << "Error on line " << node[name].Mark().line + 1 << " of provided input.";
    errors << appendErrorMessage(res, msg.str());
  }

  for (const auto & [block_name, _] : _block_templates)
  {
    errors << parseBlocks(node, block_name);
  }

  for (const auto & [block_name, typed_blocks] : _typed_block_templates)
  {
    for (const auto & [block_type, _] : typed_blocks)
    {
      errors << parseTypedBlocks(node, block_name, block_type);
    }
  }

  if (errors.str().empty())
    return "";

  if (!errors.str().empty() && filepath.empty())
    return errors.str() + "\n";

  return errorMessage("Failed to parse inputs in file \"" + filepath + "\"\n\n") + errors.str();
}

void
InputParameters::duplicateParamChecker(const std::string & name) const noexcept(false)
{
  if (_params.count(name) == 0 && _block_templates.count(name) == 0 &&
      _typed_block_templates.count(name) == 0)
    return;

  throw std::invalid_argument("Could not add  parameter with name \"" + name +
                              "\" another input already has this name");
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
InputParameters::addRepeatedBlock(const std::string & name,
                                  const InputParameters & params,
                                  const std::string & file,
                                  const std::string & function,
                                  const int line)
{

  if (_params.count(name) != 0)
  {

    std::string msg = "\n" + errorMessage("Unable to add repeated block with name \"" + name +
                                          "\" there is already a parameter with this name");

    const auto & param = _params.at(name);
    if (!param->file().empty() && !param->function().empty() && param->lineNumber() != -1)
      msg += ("\n" + errorWithContext("Previous declaration location",
                                      param->file().c_str(),
                                      param->lineNumber(),
                                      param->function().c_str()));
    throw std::invalid_argument(msg);
  }

  if (_block_templates.count(name) != 0)
  {
    std::string msg = "\n" + errorMessage("Unable to add repeated block with name \"" + name +
                                          "\" there is already a block with this name");
    throw std::invalid_argument(msg);
  }

  if (_typed_block_templates.count(name) != 0)
  {
    std::string msg;
    msg += "\n" + errorMessage("Unable to add repeated block with name \"" + name + "\"" +
                               " there is already a typed block with this name");
    throw std::invalid_argument(msg);
  }

  _block_templates[name] = params.cloneTemplate();
  _block_templates[name]->_file = file;
  _block_templates[name]->_function = function;
  _block_templates[name]->_line = line;
  _blocks[name] = std::vector<std::unique_ptr<inputs::InputParameters>>{};
}

void
InputParameters::addRequiredRepeatedBlock(const std::string & name,
                                          const InputParameters & params,
                                          const std::string & file,
                                          const std::string & function,
                                          const int line)
{
  try
  {
    addRepeatedBlock(name, params, file, function, line);
    _required_blocks.insert(name);
  }
  catch (const std::exception & e)
  {

    throw std::invalid_argument(
        "\n" + errorMessage("Unable to add required repeated typed block with name \"" + name +
                            "\"" + e.what()));
  }
}

void
InputParameters::addRepeatedTypedBlock(const std::string & name,
                                       const std::string & type,
                                       const InputParameters & params,
                                       const std::string & file,
                                       const std::string & function,
                                       const int line)
{
  if (_params.count(name) != 0)
  {

    std::string msg = "\n" + errorMessage("Unable to add repeated block with name \"" + name +
                                          "\" there is already a parameter with this name");

    const auto & param = _params.at(name);
    if (!param->file().empty() && !param->function().empty() && param->lineNumber() != -1)
      msg += ("\n" + errorWithContext("Previous declaration location",
                                      param->file().c_str(),
                                      param->lineNumber(),
                                      param->function().c_str()));
    throw std::invalid_argument(msg);
  }

  if (_block_templates.count(name) != 0)
  {
    auto & param = _block_templates[name];
    std::string msg = "\n" + errorMessage("Unable to add repeated typed block with name \"" + name +
                                          "\" there is already an untyped block with this name");

    if (!file.empty() && !function.empty() && line != -1)
      msg += "\n" + errorWithContext(
                        "Attempted declaration location", file.c_str(), line, function.c_str());

    if (!param->_file.empty() && !param->_function.empty() && param->_line != -1)
      msg += ("\n" + errorWithContext("Previous declaration location",
                                      param->_file.c_str(),
                                      param->_line,
                                      param->_function.c_str()));

    throw std::invalid_argument(msg);
  }

  if (_typed_block_templates.count(name) != 0 && _typed_block_templates.at(name).count(type) != 0)
  {
    auto & param = _typed_block_templates[name][type];
    std::string msg;
    msg += "\n" +
           errorMessage("Unable to add repeated typed block with name \"" + name + "\"" +
                        " and type \"" + type + "\" another repeated typed block already exists");

    if (!file.empty() && !function.empty() && line != -1)
      msg += "\n" + errorWithContext(
                        "Attempted declaration location", file.c_str(), line, function.c_str());

    if (!param->_file.empty() && !param->_function.empty() && param->_line != -1)
      msg += ("\n" + errorWithContext("Previous declaration location",
                                      param->_file.c_str(),
                                      param->_line,
                                      param->_function.c_str()));

    throw std::invalid_argument(msg);
  }

  _typed_block_templates.try_emplace(name);
  _typed_block_templates[name][type] = params.cloneTemplate();
  _typed_block_templates[name][type]->addRequiredParam<std::string>("type",
                                                                    "the type for this block");

  _typed_block_templates[name][type]->_file = file;
  _typed_block_templates[name][type]->_function = function;
  _typed_block_templates[name][type]->_line = line;
  _typed_blocks[name][type] = std::vector<std::unique_ptr<InputParameters>>{};
}

void
InputParameters::addRequiredRepeatedTypedBlock(const std::string & name,
                                               const std::string & type,
                                               const InputParameters & params,
                                               const std::string & file,
                                               const std::string & function,
                                               const int line)
{
  // TODO: add some checks to make sure that they did not try to register a block within the untyped
  // section typed and untyped blocks cannot share names
  try
  {
    addRepeatedTypedBlock(name, type, params, file, function, line);
    _required_blocks.insert(name);
    _required_typed_blocks.try_emplace(name);
    _required_typed_blocks[name].insert(type);
  }
  catch (const std::exception & e)
  {

    throw std::invalid_argument(
        "\n" +
        errorMessage("Unable to add required repeated typed block with name \"" + name + "\"" +
                     " and type \"" + type + "\"") +
        e.what());
  }
}

void
InputParameters::addParams(const InputParameters & params)
{
  for (const auto & [name, param] : params._params)
    _params[name] = param->cloneTemplate();

  for (const auto & name : params._required_blocks)
    _required_blocks.insert(name);

  for (const auto & [name, block_template_ptr] : params._block_templates)
  {
    _blocks[name] = std::vector<std::unique_ptr<inputs::InputParameters>>{};
    _block_templates[name] = block_template_ptr->cloneTemplate();
  }

  for (const auto & [block, types] : params._required_typed_blocks)
  {
    for (const auto & type : types)
    {
      _required_typed_blocks[block].insert(type);
    }
  }

  for (const auto & [block, type_template_map] : params._typed_block_templates)
  {
    for (const auto & [type, template_ptr] : type_template_map)
    {
      _typed_block_templates[block][type] = template_ptr->cloneTemplate();
      _typed_blocks[block][type] = std::vector<std::unique_ptr<InputParameters>>{};
    }
  }
}
const std::string
InputParameters::listParameters(const std::string & prefix) const noexcept
{
  std::stringstream params;

  if (!_params.empty())
  {
    params << prefix << "Parameters:\n";
    for (const auto & [name, param_template] : _params)
    {
      params << prefix << "  " << name << " : " << param_template->typeName();
      if (param_template->required())
      {
        params << " : required ";
      }

      params << "\n" << prefix << "    " << param_template->description() << "\n";
    }
  }

  if (!_block_templates.empty())
  {
    params << prefix << "\n\nRepeated Blocks:\n";
    for (const auto & [name, block_template] : _block_templates)
    {
      params << prefix << "  " << name << ":\n" << block_template->listParameters(prefix + "    ");
    }
  }

  if (!_typed_block_templates.empty())
  {
    params << prefix << "\n\nTyped Blocks:";
    for (const auto & [block_name, type_map] : _typed_block_templates)
    {
      for (const auto & [type_name, type_template] : type_map)
      {
        params << prefix << "\n  Type: " << type_name << "\n"
               << type_template->listParameters(prefix + "    ");
      }
    }
  }
  return params.str();
}
}
