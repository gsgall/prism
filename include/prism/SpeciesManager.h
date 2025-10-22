//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//* * Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#pragma once
#include "Species.h"

#include <string>
#include <unordered_map>

#include "boost/outcome/result.hpp"

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

namespace prism
{

/**
 * The manager is responsible for validating, creating and storing species objects which exist in
 * the system
 */
class SpeciesManager
{
public:
  SpeciesManager();

  /**
   * Given a species name this will return that species id
   * if a species with that name does not currently exist in the system then a species object will
   * be created
   * @param name the name of the species for which the name is being requested
   * @returns A result object which contains the id if it was able to be created or an error
   * message if it was not able to be created
   */
  const outcome::result<const SpeciesId, const std::string> speciesId(const std::string & name,
                                                                      const bool constant) noexcept;
  const outcome::result<void, std::string>
  addLatexOverride(const std::string & species_name, const std::string & latex_override) noexcept;

  [[nodiscard]] std::string latex(const SpeciesId id) const noexcept(false);

  void addReaction(const ReactionId,
                   std::vector<SpeciesData> & reactants,
                   std::vector<SpeciesData> & products,
                   const bool rate_reaction);

  [[nodiscard]] const std::vector<Species> & species() const noexcept;
  [[nodiscard]] const Species & species(const SpeciesId id) const noexcept(false);

  [[nodiscard]] const std::vector<SpeciesId> & constantIds();
  [[nodiscard]] const std::vector<SpeciesId> & transientIds();

private:
  outcome::result<std::tuple<std::string, std::string, int>, std::string>
  trimSpeciesModifier(const std::string & name) const noexcept;

  std::string subSpeciesBase(const std::string & name) const noexcept;

  outcome::result<void, std::string> checkName(const std::string & name) const noexcept;

  outcome::result<const std::vector<SubSpeciesData>, std::string>
  decomposeSpecies(const std::string & name);

  std::vector<Species> _species;
  std::vector<SpeciesId> _constant_ids;
  std::vector<SpeciesId> _transient_ids;
  std::unordered_map<std::string, std::string> _latex_override;
  /// the mass of every species on the periodic table
  /// these are molar masses in g / mol
  std::unordered_map<std::string, double> _masses = {{"hnu", 0.0},
                                                     {"M", 1},
                                                     {"e", 5.4857990943E-4},
                                                     {"E", 5.4857990943E-4},
                                                     {"H", 1.00794},
                                                     {"D", 2.014102},
                                                     {"T", 3.01604928},
                                                     {"He", 4.002602},
                                                     {"Li", 6.941},
                                                     {"Be", 9.012182},
                                                     {"B", 10.811},
                                                     {"C", 12.0107},
                                                     {"N", 14.0067},
                                                     {"O", 15.9994},
                                                     {"F", 18.9984032},
                                                     {"Ne", 20.1797},
                                                     {"Na", 22.98976928},
                                                     {"Mg", 24.3050},
                                                     {"Al", 26.9815386},
                                                     {"Si", 28.0855},
                                                     {"P", 30.973762},
                                                     {"S", 32.065},
                                                     {"Cl", 35.453},
                                                     {"Ar", 39.948},
                                                     {"K", 39.0983},
                                                     {"Ca", 40.078},
                                                     {"Sc", 44.955912},
                                                     {"Ti", 47.867},
                                                     {"V", 50.9415},
                                                     {"Cr", 51.9961},
                                                     {"Mn", 54.938045},
                                                     {"Fe", 55.845},
                                                     {"Co", 58.933195},
                                                     {"Ni", 58.6934},
                                                     {"Cu", 63.546},
                                                     {"Zn", 65.409},
                                                     {"Ga", 69.723},
                                                     {"Ge", 72.64},
                                                     {"As", 74.92160},
                                                     {"Se", 78.96},
                                                     {"Br", 79.904},
                                                     {"Kr", 83.798},
                                                     {"Rb", 85.4678},
                                                     {"Sr", 87.62},
                                                     {"Y", 88.90585},
                                                     {"Zr", 91.224},
                                                     {"Nb", 92.90638},
                                                     {"Mo", 95.94},
                                                     {"Tc", 98},
                                                     {"Ru", 101.07},
                                                     {"Rh", 102.90550},
                                                     {"Pd", 106.42},
                                                     {"Ag", 107.8682},
                                                     {"Cd", 112.411},
                                                     {"In", 114.818},
                                                     {"Sn", 118.710},
                                                     {"Sb", 121.760},
                                                     {"Te", 126.90447},
                                                     {"I", 126.90447},
                                                     {"Xe", 131.293},
                                                     {"Cs", 132.9054519},
                                                     {"Ba", 137.327},
                                                     {"La", 138.90547},
                                                     {"Hf", 178.89},
                                                     {"Ta", 180.94788},
                                                     {"W", 183.84},
                                                     {"Re", 186.207},
                                                     {"Os", 190.23},
                                                     {"Ir", 192.217},
                                                     {"Pt", 195.084},
                                                     {"Au", 196.966569},
                                                     {"Hg", 200.59},
                                                     {"Ti", 204.3833},
                                                     {"Pb", 207.2},
                                                     {"Bi", 208.98040},
                                                     {"Po", 209},
                                                     {"At", 210},
                                                     {"Rn", 222},
                                                     {"Fr", 223},
                                                     {"Ra", 226},
                                                     {"Ac", 227},
                                                     {"Rf", 267},
                                                     {"Db", 268.12567},
                                                     {"Sg", 271.13393},
                                                     {"Bh", 272.13826},
                                                     {"Hs", 270.13429},
                                                     {"Mt", 276.15159},
                                                     {"Ds", 281.16451},
                                                     {"Rg", 280.16514},
                                                     {"Cn", 285.17712},
                                                     // Lanthoids
                                                     {"Ce", 140.116},
                                                     {"Pr", 140.90765},
                                                     {"Nd", 144.242},
                                                     {"Pm", 145},
                                                     {"Sm", 150.36},
                                                     {"Eu", 151.964},
                                                     {"Gs", 157.25},
                                                     {"Tb", 158.92535},
                                                     {"Dy", 162.500},
                                                     {"Ho", 164.93032},
                                                     {"Er", 167.259},
                                                     {"Tm", 168.93421},
                                                     {"Yb", 173.04},
                                                     {"Lu", 174.967},
                                                     // Actinoids
                                                     {"Th", 232.02806},
                                                     {"Pa", 231.03588},
                                                     {"U", 238.02891},
                                                     {"Np", 237},
                                                     {"Pu", 244},
                                                     {"Am", 241.0568293},
                                                     {"Cm", 243.0613893},
                                                     {"Bk", 247.0703073},
                                                     {"Cf", 249.0748539},
                                                     {"Es", 252.082980},
                                                     {"Fm", 257.0951061},
                                                     {"Md", 28.0984315},
                                                     {"No", 259.10103},
                                                     {"Lr", 262.10961}};
};

}
