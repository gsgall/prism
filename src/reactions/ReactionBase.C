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
#include <boost/outcome/success_failure.hpp>

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
  declareParam("reaction",
               "",
               "The equation representing the process that occurs during the reaction",
               params,
               std::string);
  declareParam("notes",
               {},
               "Any information about this reaction which is important for others to know",
               params,
               std::vector<std::string>);
  declareParam(
      "delta-eps-e", 0.0, "The change in electron energy due to this reaction", params, double);
  declareParam("delta-eps-g",
               0.0,
               "The change in background gas energy due to this reaction",
               params,
               double);
  declareRequiredCheckedParam(
      "references",
      "The cite key(s) for the publications where this reaction and/or data was taken",
      (
          [](const std::vector<std::string> & refs) -> outcome::result<void, std::string>
          {
            if (refs.empty())
            {
              return outcome::failure("The list of references for a reaction cannot be empty");
            }
            return outcome::success();
          }),
      params,
      std::vector<std::string>);

  return params;
}

ReactionBase::ReactionBase(const inputs::InputParameters & params)
  : _delta_eps_e(params.getParam<double>("delta-eps-e")),
    _delta_eps_g(params.getParam<double>("delta-eps-g")),
    _equation(params.getParam<std::string>("reaction")),
    _notes(params.getParam<std::vector<std::string>>("notes")),
    _references(params.getParam<std::vector<std::string>>("references"))
{
}

const std::string &
ReactionBase::equation() const noexcept
{
  return _equation;
}

const std::vector<std::string> &
ReactionBase::notes() const noexcept
{
  return _notes;
}

const std::vector<std::string> &
ReactionBase::references() const noexcept
{
  return _references;
}

const std::string
ReactionBase::referencesAsLatex() const noexcept
{
  std::string temp_refs = "\\cite{";
  for (size_t i = 0; i < _references.size(); i++)
  {
    temp_refs += _references[i];
    if (i != _references.size() - 1)
      temp_refs += ", ";
  }
  temp_refs += "}";
  return temp_refs;
}

const std::vector<SpeciesData> &
ReactionBase::reactantData() const noexcept
{
  return _reactants;
}

const std::vector<SpeciesData> &
ReactionBase::productData() const noexcept
{
  return _products;
}

double
ReactionBase::deltaEnergyElectron() const noexcept
{
  return _delta_eps_e;
}

double
ReactionBase::deltaEnergyGas() const noexcept
{
  return _delta_eps_g;
}

}
