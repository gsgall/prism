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
#pragma once
#include "PrismTypes.h"
#include "Reaction.h"

#include "boost/outcome/result.hpp"

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

namespace prism
{

class SpeciesManager;

class ReactionManager
{
public:
  ReactionManager(SpeciesManager & species_manager);

  const outcome::result<ReactionId, std::string> reactionId(const std::string & equation) noexcept;

  const std::vector<Reaction> & reactions() const noexcept;

private:
  const outcome::result<std::vector<SpeciesData>, std::string>
  parseReactionSide(const std::string & side) const noexcept;

  SpeciesManager & _species_manager;

  std::vector<Reaction> _reactions;
};

}
