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
class ReactionManager;
class ReactionBase
{
public:
  virtual ~ReactionBase() = default;

  static inputs::InputParameters validParams();

  ReactionBase(const inputs::InputParameters & params);

  [[nodiscard]] const std::string & equation() const noexcept;
  [[nodiscard]] const std::vector<std::string> & notes() const noexcept;
  [[nodiscard]] const std::vector<std::string> & references() const noexcept;
  [[nodiscard]] const std::string referencesAsLatex() const noexcept;
  [[nodiscard]] const std::vector<SpeciesData> & reactantData() const noexcept;
  [[nodiscard]] const std::vector<SpeciesData> & productData() const noexcept;
  [[nodiscard]] double deltaEnergyElectron() const noexcept;
  [[nodiscard]] double deltaEnergyGas() const noexcept;

protected:
  friend ReactionManager;
  ReactionId _id;
  const double _delta_eps_e;
  const double _delta_eps_g;
  const std::string _equation;
  const std::vector<std::string> _notes;
  const std::vector<std::string> _references;
  std::vector<SpeciesData> _reactants;
  std::vector<SpeciesData> _products;
};
}
