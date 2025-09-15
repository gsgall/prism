#include "ReactionRegistrar.h"
#include "inputs/InputParameters.h"

namespace prism
{

ReactionRegistrar &
ReactionRegistrar::instance()
{
  static ReactionRegistrar instance;
  return instance;
}

void
ReactionRegistrar::registerRateReaction(const std::string & /*name*/,
                                        const inputs::InputParameters & /*params*/)
{
}

void
ReactionRegistrar::registerXSecReaction(const std::string & /*name*/,
                                        const inputs::InputParameters & /*params*/)
{
}
}
