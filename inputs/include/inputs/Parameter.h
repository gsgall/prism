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

  const outcome::result<void, std::string> set(std::any value) noexcept;
  const outcome::result<void, std::string> setFromNode(const YAML::Node & node) noexcept;

  bool required() const noexcept;
  const std::string & name() const noexcept;
  const std::string & description() const noexcept;
  const std::string & typeName() const noexcept;
  const std::string & file() const noexcept;
  const std::string & function() const noexcept;
  int lineNumber() const noexcept;
  const outcome::result<std::any, std::string> value() const noexcept;

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

  std::function<const outcome::result<void, std::string>(const std::any)> _setter_validator;
  std::function<const outcome::result<std::any, std::string>(const YAML::Node &)> _type_validater;
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
};
}
