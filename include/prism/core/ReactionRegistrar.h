#pragma once
#include <string>

namespace inputs
{
class InputParameters;
}

namespace prism
{
class ReactionRegistrar
{
public:
  static ReactionRegistrar & instance();

  void registerRateReaction(const std::string & name,
                            const inputs::InputParameters & params) noexcept(false);

  void registerXSecReaction(const std::string & name,
                            const inputs::InputParameters & params) noexcept(false);

private:
  ReactionRegistrar() = default;
  ~ReactionRegistrar() = default;
  ReactionRegistrar(const ReactionRegistrar &) = delete;
  ReactionRegistrar & operator=(const ReactionRegistrar &) = delete;
  ReactionRegistrar(ReactionRegistrar &&) = delete;
  ReactionRegistrar & operator=(ReactionRegistrar &&) = delete;
};
}
