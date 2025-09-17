#pragma once

#include "RateReactionBase.h"

namespace prism
{
class ConstantRateReaction : public RateReactionBase
{
public:
  static inputs::InputParameters validParams();

  ConstantRateReaction(const inputs::InputParameters & params);

  ~ConstantRateReaction() override = default;

  virtual double sampleRate(const double T_e, const double T_g) override;

protected:
  const double _rate;
};
}
