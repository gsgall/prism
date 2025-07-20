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
#include "SpeciesFactory.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include "StringHelper.h"
#include <iomanip>
#include "PrismErrorHelper.h"
#include "boost/outcome/success_failure.hpp"

namespace prism
{

SpeciesFactory::SpeciesFactory() {}

outcome::result<SpeciesId, std::string>
SpeciesFactory::speciesId(const std::string & name)
{

  auto it = std::find_if(
      _species.begin(), _species.end(), [name](const Species & s) { return s.name() == name; });

  if (it != _species.end())
  {
    return it->id();
  }

  if (auto res = checkName(name); !res)
  {
    std::stringstream msg;
    msg << "Invalid species name " << std::quoted(name);
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }

  auto input_data = SpeciesInitialData();
  input_data.name = name;
  input_data.id = static_cast<SpeciesId>(_species.size());
  if (const auto res = decomposeSpecies(name))
    input_data.sub_species = res.value();
  else
  {
    std::stringstream msg;
    msg << "Unable to decompose species " << std::quoted(name);
    return outcome::failure(appendErrorMessage(res, msg.str()));
  }
  _species.push_back(Species(input_data));

  return input_data.id;
}

outcome::result<void, std::string>
SpeciesFactory::checkName(const std::string & name) noexcept
{
  if (name.empty())
  {
    return outcome::failure("Species names must not be empty strings.");
  }
  std::stringstream err_msg;
  if (name[0] == 'e' || name[0] == 'E')
  {
    if (name.length() > 1)
    {
      err_msg << "Species:  " << std::quoted(name) << ": Electrons cannot have modifiers.";
      return outcome::failure(errorMessage(err_msg.str()));
    }
  }
  else
  {
    if (name != "hnu" && findFirstCapital(name) == -1)
    {
      err_msg << "Species: " << std::quoted(name);
      err_msg << ": Heavy species names must be capitalized.";

      return outcome::failure(errorMessage(err_msg.str()));
    }
  }
  return outcome::success();
}

outcome::result<const std::vector<SpeciesId>, std::string>
SpeciesFactory::decomposeSpecies(const std::string & name)
{
  const auto potental_sub_names = splitByCapital(name);

  // trivial case we just have a species it is not a composed species
  // this will handle all the cases where have something like this
  // Ar4*(adslfkj)
  if (potental_sub_names.empty() || potental_sub_names.size() == 1)
  {
    return outcome::success<const std::vector<SpeciesId>>({});
  }

  auto sub_names = std::vector<std::string>();
  // preprocess all of the sub_names in order to chck for something like
  // Ar(A) this should be allowed but we would get two elements in the
  // potential list of sub names so we need to fix this
  // we will also check to make sure that there are no special characters within the subcomponetns
  // something like Ar(alpha)B(beta) should be disallowed
  for (auto it = potental_sub_names.begin(); it != potental_sub_names.end(); it++)
  {
    const auto special_idx = findFirstSpecial(*it);

    if (std::distance(it, potental_sub_names.end()) > 2)
    {
      if (special_idx == -1)
      {
        sub_names.push_back(*it);
        continue;
      }
      else
      {
        std::stringstream msg;
        msg << "Special character found within name " << std::quoted(*it);
        return outcome::failure(errorMessage(msg.str()));
      }
    }

    if (std::distance(it, potental_sub_names.end()) == 2)
    {
      if (special_idx == -1 || (*it).back() == '(')
      {
        sub_names.push_back(*it);
        continue;
      }
    }

    if (std::distance(it, potental_sub_names.end()) == 1)
    {
      if (sub_names.back().back() == '(' && (*it).back() == ')')
      {
        sub_names.back() = sub_names.back().append(*it);
        continue;
      }

      if (sub_names.back().back() == '(' && (*it).back() != ')')
      {
        std::stringstream msg;
        msg << "Invalid modifier found on species " << std::quoted("(" + *it) << ". Did you mean "
            << std::quoted("(" + *it + ")");

        return outcome::failure(errorMessage(msg.str()));
      }
    }
    sub_names.push_back(*it);
  }

  // in this case there is no decomposition required
  if (sub_names.size() == 1)
    return outcome::success<std::vector<SpeciesId>>({});

  auto sub_ids = std::vector<SpeciesId>();
  for (const auto & sub_name : sub_names)
  {

    auto base_end = findFirstNonLetter(sub_name);
    // case for no other modifiers
    if (base_end == -1)
      base_end = sub_name.length();

    auto base = sub_name.substr(0, base_end);

    if (const auto res = speciesId(base); !res)
    {
      return outcome::failure(appendErrorMessage(res, "Unable to decompose species."));
    }
    else
    {
      sub_ids.push_back(res.value());
    }
  }
  return sub_ids;
}

const std::vector<Species> &
SpeciesFactory::species() const noexcept
{
  return _species;
}

}
