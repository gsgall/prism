#include "NetworkParser.h"
#include "ReactionRegistrar.h"
#include "boost/outcome/success_failure.hpp"
#include "inputs/ParamMacros.h"

#include <fstream>
#include <functional>
#include <inputs/InputErrorHelper.h>
#include <iostream>
#include <string>

namespace prism
{
inputs::InputParameters
NetworkParser::validParams()
{
  auto params = inputs::InputParameters();

  declareRequiredCheckedParam(
      "bibliography",
      "The file which contains all of the bibtex references for the reactions in the network",
      (
          [](const std::string & file_path) -> outcome::result<void, std::string>
          {
            if (std::ifstream input_stream(file_path); !input_stream)
            {
              return outcome::failure("Could not open file with name \"" + file_path + "\"");
            }
            return outcome::success();
          }),
      params,
      std::string);

  declareParam("data-path",
               ".//",
               "The path to the directory where the files containing input data are located",
               params,
               std::string);
  declareParam(
      "constant-species",
      {},
      "A list of species in the reaction network which are assumed to have a constant density",
      params,
      std::vector<std::string>);

  auto custom_species = inputs::InputParameters();
  declareRequiredCheckedParam(
      "name",
      "The species name which you are declaring, this must start with a capital letter",
      (
          [](const std::vector<std::string> & names) -> outcome::result<void, std::string>
          {
            for (const std::string & name : names)
            {
              if (std::islower(name.front()))
                return outcome::failure("Custom species names must begin with upper case letters.");
            }
            return outcome::success();
          }),
      custom_species,
      std::vector<std::string>);

  declareRequiredCheckedParam(
      "mass",
      "The mass of the species which you are declaring",
      (
          [](const std::vector<double> & masses) -> outcome::result<void, std::string>
          {
            for (const double mass : masses)
            {
              if (mass <= 0)
                return outcome::failure("Species masses must be non-zero positive numbers.");
            }
            return outcome::success();
          }),
      custom_species,
      std::vector<double>);
  declareRepeatedBlock("custom-species", custom_species, params);

  auto lumped_species = inputs::InputParameters();

  declareRequiredCheckedParam(
      "lumped",
      "The state which all other species are being lumped into",
      (
          [](const std::string & lumped) -> outcome::result<void, std::string>
          {
            if (std::islower(lumped.front()))
              return outcome::failure("Lumped speices names must start with a capital letter");
            return outcome::success();
          }),
      lumped_species,
      std::string);

  declareRequiredCheckedParam(
      "actual",
      "The collection of species states which will be lumped into the lumped state",
      (
          [](const std::vector<std::string> & actual) -> outcome::result<void, std::string>
          {
            for (const auto & name : actual)
            {
              if (std::islower(name.front()))
                return outcome::failure("Species names must start with a captial letter");
            }
            return outcome::success();
          }),
      lumped_species,
      std::vector<std::string>);

  declareRepeatedBlock("lumped-species", lumped_species, params);

  auto latex_overides = inputs::InputParameters();

  declareRequiredCheckedParam(
      "species",
      "The species name for which you want to create a custom latex representation",
      (
          [](const std::vector<std::string> & names) -> outcome::result<void, std::string>
          {
            for (const std::string & name : names)
            {
              if (name == "e")
                continue;
              if (std::islower(name.front()))
                return outcome::failure(
                    "Non-electron species names must start wiht a captial letter.");
            }
            return outcome::success();
          }),
      latex_overides,
      std::vector<std::string>);

  declareRequiredParam("latex",
                       "The custom latex representation of the species name",
                       latex_overides,
                       std::vector<std::string>);
  declareRepeatedBlock("latex-overrides", latex_overides, params);

  params.addParams(ReactionRegistrar::instance().validParams());

  return params;
}

NetworkParser::NetworkParser() : _species_manager(), _reaction_manager(_species_manager) {}

std::string
NetworkParser::parseNetwork(const std::string & file_path)
{
  if (std::ifstream input_stream(file_path); !input_stream)
  {
    return errorMessage("Unable to open file \"" + file_path + "\"");
  }
  else
    return parseNetwork(input_stream);
}

std::string
NetworkParser::parseNetwork(std::istream & input_stream)
{
  if (!input_stream)
  {
    return errorMessage("Bad stream provided");
  }

  auto params = validParams();

  if (const auto parsing_errors = params.parseInput(input_stream); !parsing_errors.empty())
  {
    return parsing_errors;
  }

  std::stringstream errors;
  for (const auto & species_name : params.getParam<std::vector<std::string>>("constant-species"))
  {
    if (const auto res = _species_manager.speciesId(species_name, true); !res)
      errors << res.error();
  }

  for (const auto & type : ReactionRegistrar::instance().rateTypes())
  {
    for (const auto & block : params.getTypedBlocks("rate-based", type))
    {
      if (const auto res = _reaction_manager.reactionId(block, true); !res)
        errors << res.error();
    }
  }

  return errors.str();
}

}
