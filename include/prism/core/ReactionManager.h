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

#include "inputs/InputParameters.h"
#include "RateReactionBase.h"
#include <memory>

namespace prism
{

class SpeciesManager;
class ReactionManager
{
public:
  ReactionManager(SpeciesManager & species_manager);

  const outcome::result<ReactionId, std::string>
  reactionId(const std::unique_ptr<inputs::InputParameters> & params, bool rate_reaction) noexcept;

  [[nodiscard]] const std::vector<std::unique_ptr<RateReactionBase>> &
  rateReactions() const noexcept;

private:
  const outcome::result<std::vector<SpeciesData>, std::string>
  parseReactionSide(const std::string & side) const noexcept;

  SpeciesManager & _species_manager;

  std::vector<std::unique_ptr<RateReactionBase>> _rate_reactions;
};

}
