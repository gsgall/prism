#include "Parameter.h"
#include "boost/outcome/result.hpp"
#include <sstream>
#include <string>
#include "yaml-cpp/yaml.h"

namespace input
{

template <>
std::string
typeName<unsigned int>()
{
  return "unsigned int";
}

template <>
std::string
typeName<int>()
{
  return "int";
}

template <>
std::string
typeName<std::string>()
{
  return "std::string";
}

template <>
std::string
typeName<double>()
{
  return "double";
}

template <>
std::string
typeName<bool>()
{
  return "bool";
}

template <typename T>
std::string
parseErrorMessage(const std::string & param_name, const YAML::Node & node, const bool vector)
{
  std::ostringstream oss;
  oss << "Parse error on line: " << node.Mark().line + 1 << " column: " << node.Mark().column + 1
      << "\n  ";
  oss << std::quoted(param_name) << " was unable to be parsed as a ";

  if (vector)
    oss << "std::vector<";

  oss << typeName<T>();

  if (vector)
    oss << ">";

  oss << "\n    " << node << "\n";
  return oss.str();
}

std::string
missingParamErrorMessage(const std::string & param_name)
{
  std::ostringstream oss;
  oss << "Parameter: " << std::quoted(param_name) << " was not provided\n    ";

  return oss.str();
}

ParameterBase::ParameterBase(const std::string name,
                             const std::string description,
                             const bool required)
  : _name(name), _description(description), _required(required)
{
}

template <typename T>
Parameter<T>::Parameter(const std::string name, const std::string description)
  : ParameterBase(name, description, true)
{
}

template <typename T>
Parameter<T>::Parameter(const std::string name,
                        const T default_value,
                        const std::string description)
  : ParameterBase(name, description, false), _value(default_value)
{
}

template <typename T>
Parameter<std::vector<T>>::Parameter(const std::string name, const std::string description)
  : ParameterBase(name, description, true)
{
}

template <typename T>
Parameter<std::vector<T>>::Parameter(const std::string name,
                                     const std::vector<T> default_value,
                                     const std::string description)
  : ParameterBase(name, description, false), _value(default_value)
{
}

template <typename T>
outcome::result<void, std::string>
Parameter<T>::parseInput(const YAML::Node & node)
{
  if (!node[_name].IsDefined())
  {
    return outcome::failure(missingParamErrorMessage(_name));
  }

  if (!node[_name].IsScalar())
  {
    return outcome::failure(parseErrorMessage<T>(_name, node, false));
  }

  try
  {
    _value = node[_name].template as<T>();
    return outcome::success();
  }
  catch (const YAML::BadConversion)
  {
    return outcome::failure(parseErrorMessage<T>(_name, node, false));
  }
}

template <typename T>
outcome::result<void, std::string>
Parameter<std::vector<T>>::parseInput(const YAML::Node & node)
{
  std::ostringstream oss;
  if (!node[_name].IsDefined())
  {
    return outcome::failure(missingParamErrorMessage(_name));
  }

  // we can try and let them provide a single element vector as a scalar
  if (node[_name].IsScalar())
  {
    try
    {
      _value = {node[_name].template as<T>()};
      return outcome::success();
    }
    catch (const YAML::BadConversion & e)
    {
      return outcome::failure(parseErrorMessage<T>(_name, node, true));
    }
  }
  // if the parameter is provided as a vector then we will try to get that
  try
  {
    _value = node[_name].template as<std::vector<T>>();
  }
  catch (const YAML::BadConversion & e)
  {
    return outcome::failure(parseErrorMessage<T>(_name, node, true));
  }

  return outcome::success();
}

template class Parameter<int>;
template class Parameter<bool>;
template class Parameter<double>;
template class Parameter<unsigned int>;
template class Parameter<std::string>;
template class Parameter<std::vector<int>>;
template class Parameter<std::vector<double>>;
template class Parameter<std::vector<bool>>;
template class Parameter<std::vector<unsigned int>>;
template class Parameter<std::vector<std::string>>;

}
