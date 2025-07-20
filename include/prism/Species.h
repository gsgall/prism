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
 * Struct for a quick way to access which reactions the species is in
 * Since we keep track of rate_based and xsec_based reactions seperately
 * there will be two sets of ids
 * a set of ids from 0-(n-1) for rate-based reactions (corrisponding to their index in the)
 * vector you can get from rateBasedReactions
 * the same is true for cross section based reactions
 */
struct ReactionData
{
  /// the id of a given reaction that this species is a part of
  ReactionId id;
  /// the species stoichiometry for the species in the reaction
  /// with the prescribed if
  int stoic_coeff;
};

struct SpeciesInitialData
{
  SpeciesId id;
  std::string name;
  std::vector<SpeciesId> sub_species;
};
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

  /**
   * Getter method for the name of a species object
   * @return the name of the current species
   */
  SpeciesId id() const noexcept;

  const std::vector<SpeciesId> & subSpecies() const noexcept;

private:
  SpeciesId _id;
  std::vector<SpeciesId> _sub_species;
  const std::string _name;
};
}
