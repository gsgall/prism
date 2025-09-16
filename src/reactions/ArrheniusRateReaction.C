#include "ArrheniusRateReaction.h"

namespace prism
{
registerRateReaction(ArrheniusRateReaction);

inputs::InputParameters
ArrheniusRateReaction::validParams()
{
  auto params = RateReactionBase::validParams();
  params.addDescription(
      "Reaction rate is in the form of the two temperature Arrhenius rate equation.");
  declareRequiredCheckedParam(
      "rate-constants",
      "A map containing all of the rate constants required to evaluate the two "
      "temperature Arrhenius rate equation",
      (
          [](const std::unordered_map<std::string, double> & constants)
              -> outcome::result<void, std::string>
          {
            if (constants.count("A") == 0)
              return outcome::failure("The parameter \"A\" must be provided.");
            if (constants.at("A") <= 0)
              return outcome::failure("The parameter \"A\" must be >= 0.");
            std::unordered_set<std::string> required_keys = {"A", "n_e", "E_e", "n_g", "E_g"};
            std::unordered_set<std::string> provided_keys;
            for (const auto & [key, _] : constants)
            {
              if (required_keys.count(key) == 0)
                return outcome::failure("An unknown key \"" + key + "\" was provided");
              provided_keys.emplace(key);
            }
            if (provided_keys.size() == required_keys.size())
              return outcome::failure("A required key is missing you must provide a value for \"A, "
                                      "n_e, E_e, n_g and E_g\".");

            return outcome::success();
          }),
      params,
      std::unordered_map<std::string, double>);
  return params;
}

ArrheniusRateReaction::ArrheniusRateReaction(const inputs::InputParameters & params)
  : RateReactionBase(params),
    _A(params.getParam<std::unordered_map<std::string, double>>("rate-constants").at("A")),
    _n_e(params.getParam<std::unordered_map<std::string, double>>("rate-constants").at("n_e")),
    _E_e(params.getParam<std::unordered_map<std::string, double>>("rate-constants").at("E_e")),
    _n_g(params.getParam<std::unordered_map<std::string, double>>("rate-constants").at("n_g")),
    _E_g(params.getParam<std::unordered_map<std::string, double>>("rate-constants").at("E_g"))
{
}

double
ArrheniusRateReaction::sampleRate(const double T_e, const double T_g)
{
  return _A * std::pow(T_e / 0.025, _n_e) * std::exp(-_E_e / T_e) * std::pow(T_g / 0.025, _n_g) *
         std::exp(-_E_g / T_g);
}
}
