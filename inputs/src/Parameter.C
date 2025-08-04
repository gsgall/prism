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
#include "Parameter.h"
#include "InputErrorHelper.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <unordered_set>

#include "TypeNameHelper.h"
/**
 * Minimal list of required inscludes for all of the node functionality used
 */
///@{
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/emit.h"
///@}

namespace inputs
{

ParameterBase::ParameterBase(const std::string & name,
                             const std::string & description,
                             const std::string & type_name,
                             const std::string & file,
                             const std::string & function,
                             const int line_number)
  : _name(name),
    _description(description),
    _type_name(type_name),
    _file(file),
    _function(function),
    _line_number(line_number)
{
}

const std::string &
ParameterBase::file() const noexcept
{
  return _file;
}

const std::string &
ParameterBase::function() const noexcept
{
  return _function;
}

int
ParameterBase::lineNumber() const noexcept
{
  return _line_number;
}

const std::string &
ParameterBase::typeName() const noexcept
{
  return _type_name;
}

bool
ParameterBase::required() const noexcept
{
  return !_default_value.has_value();
}

const std::string &
ParameterBase::name() const noexcept
{
  return _name;
}

const std::string &
ParameterBase::description() const noexcept
{
  return _description;
}

const outcome::result<std::any, std::string>
ParameterBase::value() const noexcept
{
  if (!_value.has_value())
  {
    std::stringstream msg;
    msg << "Attempted to retrieve the value of " << std::quoted(_name) << " before it was set";
    return outcome::failure(errorMessage(msg.str()));
  }

  return outcome::success<std::any>(_value.value());
}

template <typename T>
Parameter<T>::Parameter(
    const std::string & name,
    std::optional<T> default_val,
    const std::string & description,
    std::function<const outcome::result<void, std::string>(const T &)> validator,
    const std::string & file,
    const std::string & function,
    const int line_number)
  : ParameterBase(name, description, utils::typeName<T>(), file, function, line_number)
{
  if (default_val.has_value())
  {
    _default_value.emplace(default_val.value());
    _value.emplace(default_val.value());
  }

  _additional_validater =
      [validator](const std::any & val) -> const outcome::result<void, std::string>
  {
    if (const auto res = validator(std::any_cast<T>(val)); !res)
      return outcome::failure(errorMessage(res.error()));
    return outcome::success();
  };
}

template <typename T>
const outcome::result<void, std::string>
Parameter<T>::set(std::any value) noexcept
{
  try
  {
    _value = std::any_cast<T>(value);
    return outcome::success();
  }
  catch (const std::bad_any_cast & e)
  {
    std::stringstream msg;
    msg << "Unable to set parameter with provided value. Cast to type "
        << std::quoted(utils::typeName<T>()) << " failed.";
    return outcome::failure(errorMessage(msg.str()));
  }
}

template <typename T>
const outcome::result<void, std::string>
Parameter<T>::setFromNode(const YAML::Node & node) noexcept
{
  if (!node[_name].IsDefined())
  {
    std::stringstream msg;
    msg << "Error around line " << node.Mark().line + 1 << ". ";
    msg << "Parameter " << std::quoted(_name) << " not found in input";
    return outcome::failure(errorMessage(msg.str()));
  }

  try
  {
    // At this point we will check to make sure we can parse the node as the intended type
    _value = std::any(node[_name].template as<T>());
  }
  catch (const std::exception & e)
  {
    std::stringstream msg;
    msg << "Error on line " << node[_name].Mark().line + 1 << ". ";
    msg << "Parameter " << std::quoted(_name) << " with contents \"" << node[_name]
        << "\" is invalid.";
    msg << " Could not parse as type " << std::quoted(utils::typeName<T>()) << std::endl;
    return outcome::failure(errorMessage(msg.str()) + "\n");
  }

  // We still need to check for duplicate keys in map types since yaml-cpp does not
  // handle this
  if constexpr (std::is_same_v<T, std::unordered_map<std::string, int>> ||
                std::is_same_v<T, std::unordered_map<std::string, unsigned int>> ||
                std::is_same_v<T, std::unordered_map<std::string, double>> ||
                std::is_same_v<T, std::unordered_map<std::string, std::string>>)
  {
    std::unordered_set<std::string> keys;
    for (const auto & pair : node)
    {
      if (keys.count(pair.first.as<std::string>()) != 0)
      {
        std::stringstream msg;
        msg << "Duplicate key " << std::quoted(pair.first.as<std::string>()) << " found in node \""
            << node << "\".";
        return outcome::failure(errorMessage(msg.str()));
      }
      keys.insert(pair.first.as<std::string>());
    }
  }

  if (const auto res = _additional_validater(_value.value()); !res)
  {
    std::stringstream msg;
    msg << "Error on line " << node[_name].Mark().line + 1 << ". ";
    msg << "Parameter " << std::quoted(_name) << " with contents \"" << node[_name]
        << "\" is invalid.";
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  return outcome::success();
}

template <typename T>
std::unique_ptr<ParameterBase>
Parameter<T>::cloneTemplate() const noexcept
{
  Parameter<T> cloned = *this;
  cloned._value = cloned._default_value;
  return std::make_unique<Parameter<T>>(cloned);
}

template class Parameter<int>;
template class Parameter<bool>;
template class Parameter<double>;
template class Parameter<std::string>;
template class Parameter<unsigned int>;
template class Parameter<std::vector<int>>;
template class Parameter<std::vector<double>>;
template class Parameter<std::vector<std::string>>;
template class Parameter<std::vector<unsigned int>>;
template class Parameter<std::unordered_map<std::string, int>>;
template class Parameter<std::unordered_map<std::string, double>>;
template class Parameter<std::unordered_map<std::string, std::string>>;
template class Parameter<std::unordered_map<std::string, unsigned int>>;
}
