#pragma once

#include "inputs/InputParameters.h"
#include "SpeciesManager.h"
#include "ReactionManager.h"

namespace prism
{
class NetworkParser
{
public:
  static inputs::InputParameters validParams();

  NetworkParser();

  [[nodiscard]] std::string parseNetwork(const std::string & file_name);
  [[nodiscard]] std::string parseNetwork(std::istream & input_stream);

  [[nodiscard]] const std::vector<Species> & species() { return _species_manager.species(); }

  [[nodiscard]] const std::vector<SpeciesId> & constantSpeciesIds()
  {
    return _species_manager.constantIds();
  }

  [[nodiscard]] const std::vector<SpeciesId> & transientSpeciesIds()
  {
    return _species_manager.transientIds();
  }

private:
  SpeciesManager _species_manager;
  ReactionManager _reaction_manager;
};
}
