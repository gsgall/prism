#pragma once
#include "ReactionBase.h"

namespace prism
{
class XSecReactionBase : public ReactionBase
{
public:
  static inputs::InputParameters validParams();
  XSecReactionBase(const inputs::InputParameters & params);
  ~XSecReactionBase() override = default;

  virtual const std::string & fileLocation();

private:
  const std::string _file_location;
};
}
