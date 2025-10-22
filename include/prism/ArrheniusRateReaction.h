#pragma once

#include "RateReactionBase.h"

namespace prism
{
class ArrheniusRateReaction : public RateReactionBase
{
public:
  static inputs::InputParameters validParams();

  ArrheniusRateReaction(const inputs::InputParameters & params);

  ~ArrheniusRateReaction() override = default;

  virtual double sampleRate(const double T_e, const double T_g) override;

protected:
  const double _A;
  const double _n_e;
  const double _E_e;
  const double _n_g;
  const double _E_g;
};
}
