//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit * * Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include "Species.h"
#include "PrismConstants.h"

namespace prism
{

Species::Species(const SpeciesInitialData & data)
  : _id(data.id),
    _charge(data.charge),
    _mass(data.mass),
    _name(data.name),
    _modifier(data.modifier),
    _sub_species_data(data.sub_species_data)
{
}

const std::string &
Species::name() const noexcept
{
  return _name;
}

const std::string &
Species::modifier() const noexcept
{
  return _modifier;
}

SpeciesId
Species::id() const noexcept
{
  return _id;
}

double
Species::charge() const noexcept
{
  return _charge * ELEMENTAL_CHARGE;
}

int
Species::chargeNumber() const noexcept
{
  return _charge;
}

const std::vector<SubSpeciesData> &
Species::subSpeciesData() const noexcept
{
  return _sub_species_data;
}

double
Species::molarMass() const noexcept
{
  return _mass;
}

const std::vector<ReactionData> &
Species::rateReactionData() const noexcept
{
  return _rate_reaction_data;
}

const std::vector<ReactionData> &
Species::xsecReactionData() const noexcept
{
  return _xsec_reaction_data;
}

}

// bool
// Species::operator==(const Species & other) const
//{
//  if (_sub_species.size() != other._sub_species.size())
//    return false;
//
//  if (abs(_mass - other._mass) > numeric_limits<float>::epsilon())
//    return false;
//
//  if (_charge_num != other._charge_num)
//    return false;
//
//  if (_latex_name != other._latex_name)
//    return false;
//
//  for (unsigned int i = 0; i < _sub_species.size(); ++i)
//    if (_sub_species[i] != other._sub_species[i])
//      return false;
//
//  return true;
//}
//
// bool
// Species::operator!=(const Species & other) const
//{
//  return !(*this == other);
//}

// string
// Species::to_string() const
//{
//  std::ostringstream string_rep;
//  string_rep << endl << "Species: " << _name << endl;
//  string_rep << "  composition: " << endl;
//  for (const auto & sub : _sub_species)
//    string_rep << "    " << sub.name() << endl;
//  string_rep << "  id: " << _id << endl;
//  string_rep << SpeciesBase::to_string();
//  string_rep << "  marked constant: " << (_marked_constant ? "true" : "false") << endl;
//
//  string_rep << "  rate based reactions: " << _rate_based.size() << endl;
//  for (const auto & r_wp : _rate_based)
//    string_rep << "    " << r_wp.lock()->expression() << endl;
//
//  string_rep << "  xsec based reactions: " << _xsec_based.size() << endl;
//  for (const auto & r_wp : _xsec_based)
//    string_rep << "    " << r_wp.lock()->expression() << endl;
//
//  string_rep << endl;
//  return string_rep.str();
//}

// string
// to_string(const std::shared_ptr<prism::Species> & s)
//{
//  return s->to_string();
//}
//
// string
// to_string(const std::shared_ptr<const prism::Species> & s)
//{
//  return s->to_string();
//}
//
// std::ostream &
// operator<<(std::ostream & os, const std::shared_ptr<prism::Species> & s)
//{
//
//  os << s->to_string();
//  return os;
//}
//
// std::ostream &
// operator<<(std::ostream & os, const std::shared_ptr<const prism::Species> & s)
//{
//  os << s->to_string();
//  return os;
//}
//
//}
//
// size_t
// hash<prism::Species>::operator()(const prism::Species & obj) const
//{
//  constexpr size_t hash_factor = 37;
//
//  size_t val = 17; // Start with a prime number
//
//  for (auto s : obj.subSpecies())
//    val += hash_factor * hash<prism::SubSpecies>()(s);
//
//  val += hash_factor * hash<float>()(obj.mass());
//  val += hash_factor * hash<int>()(obj.chargeNumber());
//  val += hash_factor * hash<string>()(obj.latexRepresentation());
//  // not including the sources and sinks in the hash since those
//  // can change as reactions are added to the network
//  return val;
//}
