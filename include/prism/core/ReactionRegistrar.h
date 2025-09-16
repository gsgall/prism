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
  char
  addRateReaction(const std::string & name,
                  std::function<inputs::InputParameters()> param_func,
                  std::function<std::unique_ptr<RateReactionBase>(const inputs::InputParameters &)>
                      constructor) noexcept(false);
  std::unique_ptr<RateReactionBase> constructRateReaction(const std::string & type,
                                                          const inputs::InputParameters & params);

private:
  ReactionRegistrar();
  ~ReactionRegistrar() = default;
  ReactionRegistrar(const ReactionRegistrar &) = delete;
  ReactionRegistrar & operator=(const ReactionRegistrar &) = delete;
  ReactionRegistrar(ReactionRegistrar &&) = delete;
  ReactionRegistrar & operator=(ReactionRegistrar &&) = delete;

  inputs::InputParameters _params;
  std::unordered_map<std::string, std::function<inputs::InputParameters()>> _rate_parameters;
  std::unordered_map<
      std::string,
      std::function<std::unique_ptr<RateReactionBase>(const inputs::InputParameters &)>>
      _rate_constructors;
};
}
