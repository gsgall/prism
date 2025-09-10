//* this file is a part of prism: plasma reaction input system,
//* a library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/ncsu-comps-group/prism
//*
//* licensed under mit, please see license for details
//* https://opensource.org/license/mit
//*
//* copyright 2024, north carolina state university
//* all rights reserved
//*
#include "Reaction.h"

namespace prism
{
Reaction::Reaction(const ReactionInitialData data)
  : _id(data.id), _equation(data.equation), _reactants(data.reactants), _products(data.products),
{
}

const std::string &
Reaction::equation() const noexcept
{
  return _equation;
}

const std::vector<SpeciesData> &
Reaction::reactants() const noexcept
{
  return _reactants;
}

const std::vector<SpeciesData> &
Reaction::products() const noexcept
{
  return _products;
}

}
