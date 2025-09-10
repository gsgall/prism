#include "PrismTypes.h"

namespace inputs
{
namespace utils
{
template <>
std::string
typeName<prism::SpeciesData>()
{
  return "SpeciesData";
}

template <>
std::string
typeName<std::vector<prism::SpeciesData>>()
{
  return "std::vector<SpeciesData>";
}
}
}
