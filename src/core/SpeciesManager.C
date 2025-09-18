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
#include "SpeciesManager.h"
#include "StringHelper.h"
#include "PrismErrorHelper.h"
#include "boost/outcome/success_failure.hpp"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <locale>
#include <sstream>
#include <iomanip>
#include <string>
#include <tuple>

namespace prism
{

SpeciesManager::SpeciesManager() {}

Species &
SpeciesManager::speciesById(const SpeciesId id)
{
  return _species.at(id);
}

void
SpeciesManager::addReaction(const ReactionId id,
                            const std::vector<SpeciesData> & reactants,
                            const std::vector<SpeciesData> & products,
                            const bool rate_reaction)
{
  std::unordered_map<SpeciesId, int> stoic_map;

  for (const SpeciesData & data : reactants)
  {
    if (stoic_map.count(data.id) == 0)
      stoic_map[data.id] = 0;
    stoic_map[data.id] -= static_cast<int>(data.occurances);
  }

  for (const SpeciesData & data : products)
  {
    if (stoic_map.count(data.id) == 0)
      stoic_map[data.id] = 0;
    stoic_map[data.id] += static_cast<int>(data.occurances);
  }

  for (const auto & [species_id, stoic_coeff] : stoic_map)
  {
    if (rate_reaction)
    {
      auto & data = _species[species_id]._rate_reaction_data.emplace_back();
      data.id = id;
      data.stoic_coeff = stoic_coeff;
    }
    else
    {
      auto & data = _species[species_id]._xsec_reaction_data.emplace_back();
      data.id = id;
      data.stoic_coeff = stoic_coeff;
    }
  }
}

const outcome::result<const SpeciesId, const std::string>
SpeciesManager::speciesId(const std::string & name, const bool constant) noexcept
{
  const auto it = std::find_if(
      _species.begin(), _species.end(), [name](const Species & s) { return s.name() == name; });

  if (it != _species.end())
  {
    return it->id();
  }

  if (const auto res = checkName(name); !res)
  {
    std::stringstream msg;
    msg << "Invalid species name " << std::quoted(name);
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  auto input_data = SpeciesInitialData();
  input_data.name = name;
  input_data.modifier = "";
  // base case where we have no modifier on the species object
  // if there are only letters and there is only a single capital letter then we have a base case
  // where we can create the species directly
  if (findFirstNonLetter(name) == -1 && splitByCapital(name).size() == 1)
  {
    input_data.charge = 0;
    input_data.sub_species_data = {};
    const auto it = _masses.find(input_data.name);
    if (it == _masses.end())
    {
      std::stringstream msg;
      msg << "Unable to compute species mass. No mass available for sub species "
          << std::quoted(input_data.name);
      return outcome::failure(errorMessage(msg.str()));
    }
    input_data.mass = it->second;
    input_data.id = _species.size();
    _species.emplace_back(input_data);

    if (constant &&
        std::find(_constant_ids.begin(), _constant_ids.end(), input_data.id) == _constant_ids.end())
    {
      _constant_ids.push_back(input_data.id);
      return _constant_ids.back();
    }
    // if the species was not already declared constant then we can add it
    if (std::find(_constant_ids.begin(), _constant_ids.end(), input_data.id) == _constant_ids.end())
      _transient_ids.push_back(input_data.id);

    return input_data.id;
  }

  const auto res = trimSpeciesModifier(name);
  if (!res)
  {
    std::stringstream msg;
    msg << "Unable to decompose species " << std::quoted(name);
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  input_data.mass = 0;
  input_data.modifier = std::get<1>(res.value());
  input_data.charge = std::get<2>(res.value());

  // other wise there is still some decomposition to be done
  const auto res2 = decomposeSpecies(std::get<0>(res.value()));
  if (!res2)
  {
    std::stringstream msg;
    msg << "Unable to decompose species " << std::quoted(name);
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  input_data.sub_species_data = res2.value();
  input_data.id = _species.size();

  for (const auto & sub_data : input_data.sub_species_data)
  {
    input_data.mass += static_cast<double>(sub_data.sub_script) * _species[sub_data.id].molarMass();
  }
  // now change the mass by the mass of the electron for the charge state of the species
  input_data.mass -= static_cast<double>(input_data.charge) * _masses["e"];
  _species.emplace_back(input_data);

  if (constant &&
      std::find(_constant_ids.begin(), _constant_ids.end(), input_data.id) == _constant_ids.end())
  {
    _constant_ids.push_back(input_data.id);
    return _constant_ids.back();
  }
  // if the species was not already declared constant then we can add it
  if (std::find(_constant_ids.begin(), _constant_ids.end(), input_data.id) == _constant_ids.end())
    _transient_ids.push_back(input_data.id);

  return input_data.id;
}

outcome::result<void, std::string>
SpeciesManager::checkName(const std::string & name) const noexcept
{
  if (name.empty())
  {
    return outcome::failure("Species names must not be empty strings.");
  }
  std::stringstream err_msg;
  if (name[0] == 'e' && name.length() > 1)
  {
    if (name.length() > 1)
    {
      err_msg << "Species:  " << std::quoted(name) << ": Electrons cannot have modifiers.";
      return outcome::failure(errorMessage(err_msg.str()));
    }
  }
  else
  {
    if (name != "e" && name != "hnu" && findFirstCapital(name) == -1)
    {
      err_msg << "Species: " << std::quoted(name);
      err_msg << ": Heavy species names must be capitalized.";

      return outcome::failure(errorMessage(err_msg.str()));
    }
  }
  return outcome::success();
}

outcome::result<const std::vector<SubSpeciesData>, std::string>
SpeciesManager::decomposeSpecies(const std::string & name)
{
  const auto potental_sub_names = splitByCapital(name);

  auto sub_data = std::vector<SubSpeciesData>();
  for (const auto & sub_name : potental_sub_names)
  {
    auto & data = sub_data.emplace_back();

    const auto res = speciesId(subSpeciesBase(sub_name), false);
    if (!res)
    {
      std::stringstream msg;
      msg << "Unable to get id of subspecies: " << std::quoted(subSpeciesBase(sub_name));
      return outcome::failure(appendErrorMessage(res, msg.str()));
    }

    data.id = res.value();
    const auto num_idx = findFirstNumber(sub_name);

    if (num_idx == -1)
    {
      data.sub_script = 1;
      continue;
    }

    try
    {
      data.sub_script = std::stoi(sub_name.substr(num_idx, sub_name.length()));
    }
    catch (const std::exception & e)
    {
      std::stringstream msg;
      msg << "Unable to parse species subscript "
          << std::quoted(sub_name.substr(num_idx, sub_name.length())) << ". ";
      msg << e.what();
      return outcome::failure(errorMessage(msg.str()));
    }
  }
  return sub_data;
}

std::string
SpeciesManager::subSpeciesBase(const std::string & name) const noexcept
{
  auto base_end = findFirstNonLetter(name);
  // case for no other modifiers
  if (base_end == -1)
    base_end = name.length();

  return name.substr(0, base_end);
}

outcome::result<std::tuple<std::string, std::string, int>, std::string>
SpeciesManager::trimSpeciesModifier(const std::string & name) const noexcept
{
  if (name.compare("hnu") == 0)
    return std::make_tuple(name, "", 0);

  if (name.compare("e") == 0 || name.compare("E") == 0)
    return std::make_tuple(name, "", 1);

  auto special_idx = findFirstSpecial(name);

  if (special_idx == -1)
    return std::make_tuple(name, "", 0);

  const auto trimmed_name = name.substr(0, special_idx);
  auto modifier = name.substr(special_idx, name.size());

  // we need to keep a copy of this for the sake of error messages later on
  const auto full_modifier = modifier;
  const auto res = clearBalancedSymbols(modifier);

  if (!res)
  {
    std::stringstream msg;
    msg << "Invalid modifier due to unbalanced symbols " << std::quoted(modifier);
    return outcome::failure(errorMessage(msg.str()));
  }

  modifier = res.value();

  // at this point if there are any letters in the modifer we know that we have come accross
  // a species modifer that is invlid. All text that is not a species name should be within some
  // symbols
  const auto capital_idx = findFirstCapital(modifier);
  if (capital_idx != -1)
  {
    const auto symbol_idx = findFirstSpecial(modifier);
    std::stringstream msg;
    msg << "Symbol \"" << modifier[symbol_idx]
        << "\" detected within species name. If you intended to include a captial letter within "
           "your modifier please surround it with (), [], or {}. Detected  modifier "
        << std::quoted(full_modifier);
    return outcome::failure(errorMessage(msg.str()));
  }

  // at this point we will remove any charge in formation that is potentailly on the front of the
  // string
  int charge = 0;
  if (modifier.front() == '+' || modifier.front() == '-')
  {
    charge = modifier.front() == '+' ? 1 : -1;

    modifier = modifier.substr(1, modifier.length());

    const auto charge_end = findFirstNonNumber(modifier);
    if (charge_end != -1)
    {
      try
      {
        charge *= std::stoi(modifier.substr(0, charge_end));
      }
      catch (const std::exception & e)
      {
        std::stringstream msg;
        msg << "Unable to parse species charge " << std::quoted(modifier.substr(0, charge_end))
            << ". ";
        msg << e.what();
        return outcome::failure(msg.str());
      }

      modifier = modifier.substr(charge_end, modifier.length());
    }
  }

  // now that we have checked for captial letters to suggest potentail symbols withiin the middle of
  // the species name we can check for any letters that are also in the modifer in general. Any text
  // that is not a part of a species name should be within () {} or []
  const auto idx = findFirstNonSpecial(modifier);
  if (idx != -1)
  {
    std::stringstream msg;
    msg << "Unecapsulated text detected \"" << modifier[idx]
        << "\". All modifier text should be enclosed within (), [] or {}. Detected modifier "
        << std::quoted(full_modifier);
    return outcome::failure(errorMessage(msg.str()));
  }

  return std::make_tuple(trimmed_name, full_modifier, charge);
}

const std::vector<Species> &
SpeciesManager::species() const noexcept
{
  return _species;
}

const std::vector<SpeciesId> &
SpeciesManager::constantIds()
{
  return _constant_ids;
}

const std::vector<SpeciesId> &
SpeciesManager::transientIds()
{
  return _transient_ids;
}

}
