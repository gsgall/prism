#include "RateReactionBase.h"

namespace prism
{
inputs::InputParameters
RateReactionBase::validParams()
{
  auto params = ReactionBase::validParams();
  params.addDescription("This interface provides all of the same properties as the ReactionBase "
                        "but also adds a method that can be used for sampling reaction data.");
  return params;
}

RateReactionBase::RateReactionBase(const inputs::InputParameters & params) : ReactionBase(params) {}
}
