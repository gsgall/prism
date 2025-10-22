#pragma once

#define combineNames1(X, Y) X##Y
#define combineNames(X, Y) combineNames1(X, Y)

#define registerRateReaction(name)                                                                 \
  static char combineNames(rate_reaction_dummy_##name, __COUNTER__) =                              \
      ReactionRegistrar::instance().addRateReaction(                                               \
          #name,                                                                                   \
          []() { return name::validParams(); },                                                    \
          [](const std::unique_ptr<inputs::InputParameters> & params)                              \
          { return std::unique_ptr<RateReactionBase>(new name(*params)); })
