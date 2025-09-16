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

#include "PrismTypes.h"
#include "inputs/InputParameters.h"
#include "inputs/ParamMacros.h"
#include "RegistrationMacros.h"
#include "ReactionRegistrar.h"

namespace prism
{

class ReactionBase
{

public:
  virtual ~ReactionBase() = default;

  static inputs::InputParameters validParams();

  ReactionBase(const inputs::InputParameters & params);

  const std::string & equation() const noexcept;
  const std::vector<SpeciesData> & reactants() const noexcept;
  const std::vector<SpeciesData> & products() const noexcept;

protected:
  const ReactionId _id;
  const std::string _equation;
  std::vector<SpeciesData> _reactants;
  std::vector<SpeciesData> _products;
};
}
