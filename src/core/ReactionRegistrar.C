#include "RateReactionBase.h"
#include "ReactionRegistrar.h"
#include <inputs/InputParameters.h>

namespace prism
{

ReactionRegistrar::ReactionRegistrar() {}

ReactionRegistrar &
ReactionRegistrar::instance()
{
  static ReactionRegistrar instance;
  return instance;
}

const inputs::InputParameters &
ReactionRegistrar::validParams()
{
  return params();
}

char
ReactionRegistrar::addRateReaction(
    const std::string & name,
    std::function<inputs::InputParameters()> param_func,
    std::function<std::unique_ptr<RateReactionBase>(const inputs::InputParameters &)> constructor)
{
  rateParameters()[name] = param_func;
  rateConstructors()[name] = constructor;
  declareRepeatedTypedBlock("rate-based", name, param_func(), params());
  return 0;
}

std::unique_ptr<RateReactionBase>
ReactionRegistrar::constructRateReaction(const inputs::InputParameters & params)
{
  return rateConstructors().at(params.getParam<std::string>("type"))(params);
}

}
