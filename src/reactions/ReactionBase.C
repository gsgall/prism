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
#include "ReactionBase.h"

namespace prism
{

inputs::InputParameters
ReactionBase::validParams()
{
  auto params = inputs::InputParameters();
  params.addDescription(
      "This is the base reaction object which provides all of the common information across "
      "reaction types for both rate-based reactions and cross-section based reaction.");
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
  declareParam("notes",
               {},
               "Any information about this reaction which is important for others to know",
               params,
               std::vector<std::string>);
  declareRequiredParam(
      "references",
      "The cite key(s) for the publications where this reaction and/or data was taken",
      params,
      std::vector<std::string>);

  return params;
}

ReactionBase::ReactionBase(const inputs::InputParameters & params)
  : _id(params.getParam<ReactionId>("id")),
    _equation(params.getParam<std::string>("equation")),
    _reactants(params.getParam<std::vector<SpeciesData>>("reactants")),
    _products(params.getParam<std::vector<SpeciesData>>("products"))
{
}

const std::string &
ReactionBase::equation() const noexcept
{
  return _equation;
}

const std::vector<SpeciesData> &
ReactionBase::reactants() const noexcept
{
  return _reactants;
}

const std::vector<SpeciesData> &
ReactionBase::products() const noexcept
{
  return _products;
}

}
