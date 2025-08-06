//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry *
// https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#pragma once

#include "TypeNameHelper.h"
#include <algorithm>
#include <unordered_map>
#include <any>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include "Parameter.h"
#include "InputErrorHelper.h"

namespace inputs
{

class InputParameters
{

public:
  InputParameters();

  /**
   * This constructor copies the template information from the other input parameter.
   * However, this will not copy the description to the new input parameter since this is intended
   * to be used as a way to append input parameters not create multiple of the same input parameter
   * objects
   * @param other the InputParameter object that has all of the parameters declared that you want
   * the first to have
   */
  ///@{
  InputParameters(const InputParameters & other);
  InputParameters & operator=(const InputParameters & other);
  ///@}

  InputParameters(InputParameters &&) noexcept = default;
  InputParameters & operator=(InputParameters &&) noexcept = default;

  const std::vector<std::unique_ptr<InputParameters>> & blocks(const std::string & name) const
      noexcept(false);

  void addRepeatedBlock(const std::string & name, const InputParameters & params) noexcept(false);

  void addRepeatedTypedBlock(const std::string & name,
                             const std::string & type,
                             const InputParameters & params) noexcept(false);

  void addDescription(const std::string & description) noexcept;

  const std::string & description() const noexcept;

  void readFromNodes(const YAML::Node & node) noexcept(false);

  template <typename T>
  void
  addRequiredParam(const std::string & name,
                   const std::string & description,
                   std::function<const outcome::result<void, std::string>(const T &)> validator,
                   const std::string & file = "",
                   const std::string & function = "",
                   const int line_number = -1) noexcept(false)
  {
    duplicateParamChecker(name);

    _params[name] = std::make_unique<Parameter<T>>(
        name, std::nullopt, description, validator, file, function, line_number);
  }

  template <typename T>
  void addRequiredParam(const std::string & name,
                        const std::string & description,
                        const std::string & file = "",
                        const std::string & function = "",
                        const int line_number = -1) noexcept(false)
  {
    addRequiredParam<T>(
        name,
        description,
        [](const T &) -> const outcome::result<void, std::string> { return outcome::success(); },
        file,
        function,
        line_number);
  }

  template <typename T>
  void addParam(const std::string & name,
                const T & default_value,
                const std::string & description,
                std::function<const outcome::result<void, std::string>(const T &)> validator,
                const std::string & file = "",
                const std::string & function = "",
                const int line_number = -1) noexcept(false)
  {
    duplicateParamChecker(name);

    _params[name] = std::make_unique<Parameter<T>>(
        name, default_value, description, validator, file, function, line_number);
  }

  template <typename T>
  void addParam(const std::string & name,
                const T & default_value,
                const std::string & description,
                const std::string & file = "",
                const std::string & function = "",
                const int line_number = -1) noexcept(false)

  {
    addParam<T>(
        name,
        default_value,
        description,
        [](const T &) -> const outcome::result<void, std::string> { return outcome::success(); },
        file,
        function,
        line_number);
  }

  template <typename T>
  T getParam(const std::string & name)
  {

    const auto error_message = [this, &name]() -> const std::string
    {
      std::stringstream msg;
      msg << "Parameter " << std::quoted(name) << " of type " << utils::typeName<T>()
          << " is not a valid parameter.\nValid parameters: {";
      for (auto it = _params.begin(); it != _params.end(); ++it)
      {
        const auto & [key, value] = *it;
        msg << key << " : " << value->typeName();

        if (std::next(it) != _params.end())
          msg << ", ";
      }
      msg << "}";
      return msg.str();
    };

    if (_params.count(name) == 0)
    {
      throw std::invalid_argument("\n" + errorMessage(error_message()));
    }
    const auto res = _params.at(name)->value();
    if (!res)
    {
      std::stringstream msg;
      msg << "Error when retrieving parameter " << std::quoted(name) << " as type "
          << std::quoted(utils::typeName<T>()) << ".";
      throw std::invalid_argument("\n" + appendErrorMessage(res, msg.str()));
    }

    try
    {
      return std::any_cast<T>(res.value());
    }
    catch (std::bad_any_cast & e)
    {
      std::stringstream msg;
      msg << "Parameter " << std::quoted(name) << " of type "
          << std::quoted(_params.at(name)->typeName()) << " cannot be cast to type "
          << std::quoted(utils::typeName<T>()) << ".\n " << e.what();
      throw std::invalid_argument("\n" + errorMessage(msg.str()));
    }
  }

  std::unique_ptr<InputParameters> cloneTemplate() const noexcept;

private:
  std::unordered_map<std::string, std::unique_ptr<InputParameters>> _block_templates;
  std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<InputParameters>>>
      _typed_block_templates;

  std::unordered_map<std::string, std::vector<std::unique_ptr<InputParameters>>> _blocks;
  std::unordered_map<std::string,
                     std::unordered_map<std::string, std::vector<std::unique_ptr<InputParameters>>>>
      _typed_blocks;
  /// a description for the purpose of these input parameters
  std::string _description;
  std::unordered_map<std::string, std::unique_ptr<ParameterBase>> _params;

  void duplicateParamChecker(const std::string & name) const noexcept(false);
};

}
