#pragma once
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"
#include "inputs/TypeNameHelper.h"

namespace prism
{
typedef unsigned int ReactionId;
typedef unsigned int SpeciesId;

/**
 * Struct for quickly accessing data about which speies
 * are in a reaction
 */
struct SpeciesData
{
  /// the unique id for the species (guaranteed to be in the range 0-(n-1) where n in the number unique species)
  SpeciesId id;
  /// the number of times the species occurs on a side of the reaction
  unsigned int occurances;
};
/**
 * Struct for a quick way to access which reactions the species is in
 * Since we keep track of rate_based and xsec_based reactions seperately
 * there will be two sets of ids
 * a set of ids from 0-(n-1) for rate-based reactions (corrisponding to their index in the)
 * vector you can get from rateBasedReactions
 * the same is true for cross section based reactions
 */
struct ReactionData
{
  /// the id of a given reaction that this species is a part of
  ReactionId id;
  /// the species stoichiometry for the species in the reaction
  /// with the prescribed if
  int stoic_coeff;
};

struct SubSpeciesData
{
  SpeciesId id;
  unsigned int sub_script;
};

struct SpeciesInitialData
{
  SpeciesId id;
  int charge;
  double mass;
  std::string name;
  std::string modifier;
  std::vector<SubSpeciesData> sub_species_data;
};

struct ReactionInitialData
{
  ReactionId id;
  std::string equation;
  std::vector<SpeciesData> reactants;
  std::vector<SpeciesData> products;
};
}

namespace YAML
{
template <>
struct convert<prism::SpeciesData>
{
  static Node encode(const prism::SpeciesData & rhs)
  {
    Node node;
    node["id"] = rhs.id;
    node["occurances"] = rhs.occurances;
    return node;
  }

  static bool decode(const Node & node, prism::SpeciesData & data)
  {
    if (!node.IsMap())
      return false;
    if (!node["id"].IsDefined() || !node["occurances"].IsDefined())
      return false;

    data.id = node["id"].as<unsigned int>();
    data.occurances = node["occurances"].as<unsigned int>();
    return true;
  }
};

// TODO: actually implemtn these but we really don't need them since these should be private
// parameters anway
template <>
struct convert<std::vector<prism::SpeciesData>>
{
  static Node encode(const std::vector<prism::SpeciesData> & /*rhs*/)
  {
    Node node;
    return node;
  }

  static bool decode(const Node & /*node*/, std::vector<prism::SpeciesData> & /*data*/)
  {
    return true;
  }
};
}

namespace inputs
{
namespace utils
{
template <>
std::string typeName<prism::SpeciesData>();

template <>
std::string typeName<std::vector<prism::SpeciesData>>();
}
}
