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
  declareRequiredCheckedParam("file",
                              "The location where the cross section data is stored.",
                              (
                                  [](const std::string & file) -> outcome::result<void, std::string>
                                  {
                                    if (std::ifstream input_stream(file); !input_stream)
                                    {
                                      return outcome::failure("Unable to open file \"" + file +
                                                              "\"");
                                    }
                                    return outcome::success();
                                  }),
                              params,
                              std::string);

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
