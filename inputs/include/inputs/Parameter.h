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

#include "boost/outcome/result.hpp"
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
            const int line_number = -1);

  virtual std::unique_ptr<ParameterBase> cloneTemplate() const noexcept override;
  virtual const outcome::result<void, std::string> set(std::any value) noexcept override;
  virtual const outcome::result<void, std::string>
  setFromNode(const YAML::Node & node) noexcept override;
};
}
