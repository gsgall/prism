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

}
