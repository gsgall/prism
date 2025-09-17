#define combineNames1(X, Y) X##Y
#define combineNames(X, Y) combineNames1(X, Y)

#define registerRateReaction(name)                                                                 \
  static char combineNames(rate_reaction_dummy_##name, __COUNTER__) =                              \
      ReactionRegistrar::instance().addRateReaction(                                               \
          #name,                                                                                   \
          []() { return name::validParams(); },                                                    \
          [](const inputs::InputParameters & params)                                               \
          { return std::unique_ptr<RateReactionBase>(new name(params)); })

#define registerXSecReaction(name)                                                                 \
  static char combineNames(rate_reaction_dummy_##name, __COUNTER__) =                              \
      ReactionRegistrar::instance().addXSecReaction(                                               \
          #name,                                                                                   \
          []() { return name::validParams(); },                                                    \
          [](const inputs::InputParameters & params) { return std::make_unique<name>(params); })
