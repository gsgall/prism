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

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>

#include "boost/outcome/result.hpp"
#include "inputs/InputErrorHelper.h"
#include "inputs/TypeNameHelper.h"
namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

namespace YAML
{
class Node;
}

namespace inputs
{

class ParameterBase
{

public:
  virtual ~ParameterBase() = default;

  virtual const outcome::result<void, std::string> set(std::any value) noexcept = 0;
  virtual const outcome::result<void, std::string>
  setFromNode(const YAML::Node & node) noexcept = 0;

  [[nodiscard]] bool required() const noexcept;
  [[nodiscard]] const std::string & name() const noexcept;
  [[nodiscard]] const std::string & description() const noexcept;
  [[nodiscard]] const std::string & typeName() const noexcept;
  [[nodiscard]] const std::string & file() const noexcept;
  [[nodiscard]] const std::string & function() const noexcept;
  [[nodiscard]] int lineNumber() const noexcept;
  const outcome::result<std::any, std::string> value() const noexcept;

  /**
   * Method for copying all of the member variables into the new parameter except for the _value
   * member variable
   */
  [[nodiscard]] virtual std::unique_ptr<ParameterBase> cloneTemplate() const noexcept = 0;

protected:
  ParameterBase(const std::string & name,
                const std::string & description,
                const std::string & type_name,
                const std::string & file,
                const std::string & function,
                const int line_number);
  const std::string _name;
  const std::string _description;
  const std::string _type_name;
  const std::string _file;
  const std::string _function;
  const int _line_number;
  std::optional<std::any> _value;
  std::optional<std::any> _default_value;
  std::function<const outcome::result<void, std::string>(const std::any &)> _additional_validater;
};

template <typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(const std::string & name,
            std::optional<T> default_val,
            const std::string & description,
            std::function<const outcome::result<void, std::string>(const T &)> validator,
            const std::string & file = "",
            const std::string & function = "",
            const int line_number = -1)
    : ParameterBase(name, description, utils::typeName<T>(), file, function, line_number)
  {
    if (default_val.has_value())
    {
      _default_value.emplace<T>(static_cast<T>(default_val.value()));
      _value.emplace<T>(static_cast<T>(default_val.value()));
    }

    _additional_validater =
        [validator](const std::any & val) -> const outcome::result<void, std::string>
    {
      if (const auto res = validator(std::any_cast<T>(val)); !res)
        return outcome::failure(errorMessage(res.error()));
      return outcome::success();
    };
  }

  const outcome::result<void, std::string> set(std::any value) noexcept
  {
    try
    {
      _value.emplace<T>(std::any_cast<T>(value));
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

  const outcome::result<void, std::string> setFromNode(const YAML::Node & node) noexcept
  {
    try
    {
      // At this point we will check to make sure we can parse the node as the intended type
      _value.emplace<T>(node[_name].template as<T>());
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
      for (const auto & pair : node[_name])
      {
        if (keys.count(pair.first.template as<std::string>()) != 0)
        {
          std::stringstream msg;
          msg << "Duplicate key " << std::quoted(pair.first.template as<std::string>())
              << " found in node \"" << node << "\".";
          return outcome::failure(errorMessage(msg.str()));
        }
        keys.insert(pair.first.template as<std::string>());
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

  std::unique_ptr<ParameterBase> cloneTemplate() const noexcept
  {
    Parameter<T> cloned = *this;
    cloned._value = cloned._default_value;
    return std::make_unique<Parameter<T>>(cloned);
  }
};
}
