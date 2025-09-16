#pragma once
#include "ReactionBase.h"

namespace prism
{
class RateReactionBase : public ReactionBase
{
public:
  static inputs::InputParameters validParams();
  RateReactionBase(const inputs::InputParameters & params);
  ~RateReactionBase() override = default;

  virtual double sampleRate(const double T_e, const double T_g) = 0;
};
}
