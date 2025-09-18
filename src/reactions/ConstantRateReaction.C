#include "ConstantRateReaction.h"
#include <boost/outcome/success_failure.hpp>

namespace prism
{
registerRateReaction(ConstantRateReaction);

inputs::InputParameters
ConstantRateReaction::validParams()
{
  auto params = RateReactionBase::validParams();
  params.addDescription("Provides a constant reaction rate");
  declareRequiredCheckedParam("rate",
                              "The constant rate at which place this reaction occurs",
                              (
                                  [](const double & rate) -> outcome::result<void, std::string>
                                  {
                                    if (rate <= 0)
                                    {
                                      return outcome::failure(
                                          "The provided reaction rate must be >= 0");
                                    }
                                    return outcome::success();
                                  }),
                              params,
                              double);
  return params;
}

ConstantRateReaction::ConstantRateReaction(const inputs::InputParameters & params)
  : RateReactionBase(params), _rate(params.getParam<double>("rate"))
{
}

double
ConstantRateReaction::sampleRate(const double /*T_e*/, const double /*T_g*/)
{
  return _rate;
}
}
