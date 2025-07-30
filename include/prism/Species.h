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
  const std::string & name() const noexcept;

  double charge() const noexcept;
  int chargeNumber() const noexcept;

  double molarMass() const noexcept;

  /**
   * Getter method for the name of a species object
   * @return the name of the current species
   */
  SpeciesId id() const noexcept;

  const std::vector<SubSpeciesData> & subSpeciesData() const noexcept;

private:
  SpeciesId _id;
  const int _charge;
  const double _mass;
  std::vector<SubSpeciesData> _sub_species_data;
  const std::string _name;
};
}
