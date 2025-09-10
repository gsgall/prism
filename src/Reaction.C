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
#include "inputs/InputParameters.h"
#include "inputs/ParamMacros.h"

namespace prism
{

inputs::InputParameters
Reaction::validParams()
{
  auto params = inputs::InputParameters();
  // TODO: once there is better support for private params these should all be private parameters
  declareParam("id", 0, "The reaction id for this reaction", params, ReactionId);
  declareParam("equation",
               "",
               "The equation representing the process that occurs during the reaction",
               params,
               std::string);
  declareParam("reactants",
               {},
               "The species ids and number of occurances for species which are reactants",
               params,
               std::vector<SpeciesData>);
  declareParam("products",
               {},
               "The species ids and number of occurances for species which are products",
               params,
               std::vector<SpeciesData>);

  return params;
}

Reaction::Reaction(const inputs::InputParameters & params)
  : _id(params.getParam<ReactionId>("id")),
    _equation(params.getParam<std::string>("equation")),
    _reactants(params.getParam<std::vector<SpeciesData>>("reactants")),
    _products(params.getParam<std::vector<SpeciesData>>("products"))
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
