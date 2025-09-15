#include "RateReactionBase.h"
#include "inputs/InputParameters.h"

namespace prism
{
inputs::InputParameters
RateReactionBase::validParams()
{
  return ReactionBase::validParams();
}

RateReactionBase::RateReactionBase(const inputs::InputParameters & params) : ReactionBase(params) {}
}
