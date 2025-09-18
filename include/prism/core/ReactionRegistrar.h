#pragma once
#include "inputs/InputParameters.h"
#include <functional>
#include <inputs/InputParameters.h>
#include <memory>
#include <string>

namespace inputs
{
class InputParameters;
}

namespace prism
{
class RateReactionBase;
class ReactionRegistrar
{
public:
  static ReactionRegistrar & instance();

  const inputs::InputParameters & validParams();
  char addRateReaction(
      const std::string & name,
      std::function<inputs::InputParameters()> param_func,
      std::function<std::unique_ptr<RateReactionBase>(
          const std::unique_ptr<inputs::InputParameters> &)> constructor) noexcept(false);
  std::unique_ptr<RateReactionBase>
  constructRateReaction(const std::unique_ptr<inputs::InputParameters> & params);

  const std::vector<std::string> rateTypes()
  {
    std::vector<std::string> types;
    types.reserve(rateParameters().size());
    for (const auto & [type_name, _] : rateParameters())
    {
      types.push_back(type_name);
    }
    return types;
  }

private:
  ReactionRegistrar();
  ~ReactionRegistrar() = default;
  ReactionRegistrar(const ReactionRegistrar &) = delete;
  ReactionRegistrar & operator=(const ReactionRegistrar &) = delete;
  ReactionRegistrar(ReactionRegistrar &&) = delete;
  ReactionRegistrar & operator=(ReactionRegistrar &&) = delete;

  static std::map<std::string, std::function<inputs::InputParameters()>> & rateParameters()
  {
    static std::map<std::string, std::function<inputs::InputParameters()>> rate_parameters;
    return rate_parameters;
  }

  static std::map<std::string,
                  std::function<std::unique_ptr<RateReactionBase>(
                      const std::unique_ptr<inputs::InputParameters> &)>> &
  rateConstructors()
  {
    static std::map<std::string,
                    std::function<std::unique_ptr<RateReactionBase>(
                        const std::unique_ptr<inputs::InputParameters> &)>>
        rate_constructors;
    return rate_constructors;
  }
  static inputs::InputParameters & params()
  {
    static inputs::InputParameters rate_parameters;
    return rate_parameters;
  }
};
}
