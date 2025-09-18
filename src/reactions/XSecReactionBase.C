#include "XSecReactionBase.h"
#include <fstream>

namespace prism
{
inputs::InputParameters
XSecReactionBase::validParams()
{
  auto params = ReactionBase::validParams();
  params.addDescription(
      "This interface provides all of the same properties as the ReactionBase "
      "but also adds a method for getting the location where cross section data is stored.");
  declareRequiredParam(
      "file", "The location where the cross section data is stored.", params, std::string);

  return params;
}

XSecReactionBase::XSecReactionBase(const inputs::InputParameters & params)
  : ReactionBase(params), _file_location(params.getParam<std::string>("file"))
{
}

const std::string &
XSecReactionBase::fileLocation()
{
  return _file_location;
}
}
