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
#include <sstream>
#include "StringHelper.h"
#include <iomanip>
#include "PrismErrorHelper.h"

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

    return outcome::failure(res.error());
  }

  return static_cast<SpeciesId>(_species.size());
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

const std::vector<SpeciesId>
SpeciesFactory::decomposeSpecies(const std::string & name)
{
  std::vector<std::string> temp_parts = splitByCapital(name);
  return {};

  //  for (auto part : temp_parts)
  //  {
  //    if (parts.size() == 0)
  //    {
  //      parts.push_back(part);
  //      continue;
  //    }
  //    // if the last character of the most recently added part contains '(' but no closing
  //    // paranthesis then we need to check to see if the last character of this next part is ')'
  //    // this way we consider everything in the () to be a modifier even if it contains a capital
  //    // letter example case of this is Ar(S) or even Ar(aas2d3S)
  //    if ((parts.back().find("(") != string::npos && parts.back().find(")") == string::npos) &&
  //        part.back() == ')')
  //    {
  //      // get the last element
  //      auto temp_part = parts.back();
  //      // remove the element from the list
  //      parts.pop_back();
  //      // combine the current part and the previously added part
  //      temp_part += part;
  //      // add it back to the vector
  //      parts.push_back(temp_part);
  //      continue;
  //    }
  //
  //    parts.push_back(part);
  //  }
  //
  //  vector<SubSpecies> sub_sp;
  //
  //  for (auto part : parts)
  //    sub_sp.push_back(SubSpecies(part));
  //
  //  return sub_sp;
}

}
