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
#include <string>
#include <vector>
#include "PrismTypes.h"

namespace prism
{

/**
 * The species object which represents the products
 * and reactants in the reaction
 */
class SpeciesManager;
class Species
{
public:
  /**
   * Constructor for the species based on its symbolic representation
   * @param name the symbol used for the species
   */
  Species(const SpeciesInitialData & data);
  /**
   * Getter method for the name of a species object
   * @return the name of the current species
   */
  [[nodiscard]] const std::string & name() const noexcept;
  [[nodiscard]] const std::string & baseName() const noexcept;
  [[nodiscard]] const std::string & modifier() const noexcept;
  [[nodiscard]] double charge() const noexcept;
  [[nodiscard]] int chargeNumber() const noexcept;
  [[nodiscard]] double molarMass() const noexcept;
  [[nodiscard]] const std::vector<ReactionData> & rateReactionData() const noexcept;
  [[nodiscard]] const std::vector<ReactionData> & xsecReactionData() const noexcept;

  /**
   * Getter method for the name of a species object
   * @return the name of the current species
   */
  [[nodiscard]] SpeciesId id() const noexcept;
  [[nodiscard]] const std::vector<SubSpeciesData> & subSpeciesData() const noexcept;

private:
  friend SpeciesManager;
  SpeciesId _id;
  const int _charge;
  const double _mass;
  const std::string _name;
  const std::string _base_name;
  const std::string _modifier;
  std::vector<SubSpeciesData> _sub_species_data;
  std::vector<ReactionData> _rate_reaction_data;
  std::vector<ReactionData> _xsec_reaction_data;
};
}
