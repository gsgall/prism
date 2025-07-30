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
  ParameterBase(const std::string & name, const std::string & description, const bool required);
  virtual ~ParameterBase() = default;

  const outcome::result<void, std::string> validate(const YAML::Node & node) const noexcept;

  bool required() const noexcept;
  const std::string & name() const noexcept;
  const std::string & description() const noexcept;

protected:
  const bool _required;
  const std::string _name;
  const std::string _description;

  std::function<const outcome::result<void, std::string>(const YAML::Node &)> _type_validater;
};

template <typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(const std::string & name, const std::string & description, const bool required);

  T value() const noexcept;

private:
  T _value;
};
}
