#pragma once

#include "inputs/InputParameters.h"

namespace prism
{
class NetworkParser
{
public:
  static inputs::InputParameters validParams();

  NetworkParser();

  [[nodiscard]] std::string parseNetwork(const std::string & file_name);
  [[nodiscard]] std::string parseNetwork(std::istream & input_stream);
};
}
