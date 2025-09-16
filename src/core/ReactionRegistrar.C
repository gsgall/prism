#include "RateReactionBase.h"
#include "ReactionRegistrar.h"
#include <inputs/InputParameters.h>

namespace prism
{

ReactionRegistrar::ReactionRegistrar() : _params(inputs::InputParameters()) {}

ReactionRegistrar &
ReactionRegistrar::instance()
{
  static ReactionRegistrar instance;
  return instance;
}

const inputs::InputParameters &
ReactionRegistrar::validParams()
{
  return _params;
}

char
ReactionRegistrar::addRateReaction(
    const std::string & name,
    std::function<inputs::InputParameters()> param_func,
    std::function<std::unique_ptr<RateReactionBase>(const inputs::InputParameters &)> constructor)
{
  _rate_parameters[name] = param_func;
  _rate_constructors[name] = constructor;
  declareRepeatedTypedBlock("rate-based", name, param_func(), _params);
  return 0;
}

std::unique_ptr<RateReactionBase>
ReactionRegistrar::constructRateReaction(const std::string & type,
                                         const inputs::InputParameters & params)
{
  return _rate_constructors.at(type)(params);
}

}
