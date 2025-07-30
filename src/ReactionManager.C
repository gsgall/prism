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

#include "ReactionManager.h"
#include "SpeciesManager.h"
#include "boost/outcome/success_failure.hpp"
#include "StringHelper.h"
#include "PrismErrorHelper.h"
#include <exception>
#include <iomanip>
#include <sstream>

namespace prism
{

ReactionManager::ReactionManager(SpeciesManager & species_manager)
  : _species_manager(species_manager)
{
}

const std::vector<Reaction> &
ReactionManager::reactions() const noexcept
{
  return _reactions;
}

const outcome::result<ReactionId, std::string>
ReactionManager::reactionId(const std::string & equation) noexcept
{

  const auto parts = splitByDelimiter(equation, " -> ");

  if (parts.size() == 1)
  {
    std::stringstream msg;
    msg << "Invalid equation " << std::quoted(equation);
    msg << ". A reaction must include one arrow " << std::quoted(" -> ");
    return outcome::failure(errorMessage(msg.str()));
  }

  if (parts.size() != 2)
  {
    std::stringstream msg;
    msg << "Invalid equation " << std::quoted(equation);
    msg << ". " << parts.size() - 1 << " arrows found. A reaction may only contain one arrow.";
    return outcome::failure(errorMessage(msg.str()));
  }

  auto reaction_input = ReactionInitialData();
  auto res = parseReactionSide(parts.front());

  if (!res)
  {
    std::stringstream msg;
    msg << "Failed to parse reactant side " << std::quoted(parts.front());
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  reaction_input.reactants = res.value();

  res = parseReactionSide(parts.back());
  if (!res)
  {
    std::stringstream msg;
    msg << "Failed to parse product side " << std::quoted(parts.back());
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  reaction_input.products = res.value();

  reaction_input.equation = equation;
  reaction_input.id = _reactions.size();

  _reactions.emplace_back(reaction_input);

  return reaction_input.id;
}

const outcome::result<std::vector<SpeciesData>, std::string>
ReactionManager::parseReactionSide(const std::string & side) const noexcept
{

  std::map<SpeciesId, unsigned int> species_map;
  const auto species = splitByDelimiter(side, " + ");

  for (const auto & s : species)
  {
    const auto special_idx = findFirstSpecial(s);
    const auto letter_idx = findFirstLetter(s);

    if (special_idx != -1 && special_idx < letter_idx)
    {
      std::stringstream msg;
      msg << "Non numbers " << std::quoted(s.substr(0, special_idx));
      msg << " cannot prepend species names " << std::quoted(s);
      return outcome::failure(errorMessage(msg.str()));
    }

    // at this point we should have confirmed that there are no special characters in front of a
    // species but there may still be a number which we will need to convert to an actual number
    const auto number_idx = findFirstNumber(s);
    // this will store the species name that will be parsed without a coeffiecient
    std::string trimmed_species;
    unsigned int occurances = 0;
    // this is the case where we either have no coefficient and no other numbers or we have no
    // coefficient but there are still other numbers which may be in the modifier
    if (number_idx == -1 || number_idx > letter_idx)
    {
      occurances = 1;
      trimmed_species = s;
    }
    else
    {
      try
      {
        occurances = std::stoi(s.substr(0, letter_idx));
        trimmed_species = s.substr(letter_idx, s.length());
      }
      catch (const std::exception & e)
      {
        std::stringstream msg;
        msg << "Error parsing species coefficient " << std::quoted(s.substr(0, letter_idx)) << ". ";
        msg << e.what();
      }
    }

    const auto res = _species_manager.speciesId(trimmed_species);
    if (!res)
    {
      std::stringstream msg;
      msg << "Error parsing species " << std::quoted(trimmed_species) << " on reaction side "
          << std::quoted(side) << ".";
      return outcome::failure(appendErrorMessage(res, msg.str()));
    }

    if (species_map.find(res.value()) == species_map.end())
    {
      species_map[res.value()] = occurances;
      continue;
    }
    species_map[res.value()] += occurances;
  }

  auto data = std::vector<SpeciesData>();

  for (const auto & it : species_map)
  {
    auto & s_data = data.emplace_back();
    s_data.id = it.first;
    s_data.occurances = it.second;
  }

  return data;
}

}
