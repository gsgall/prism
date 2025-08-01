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
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace inputs
{

InputParameters::InputParameters() {};

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
