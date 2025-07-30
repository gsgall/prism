#include "Parameter.h"
#include "InputErrorHelper.h"
#include <boost/outcome/result.hpp>
#include <boost/outcome/success_failure.hpp>
#include <iomanip>
#include <locale>
#include "yaml-cpp/yaml.h"

namespace inputs
{

ParameterBase::ParameterBase(const std::string & name,
                             const std::string & description,
                             const bool required)
  : _required(required), _name(name), _description(description)
{
}

bool
ParameterBase::required() const noexcept
{
  return _required;
}

const std::string &
ParameterBase::name() const noexcept
{
  return _name;
}

const std::string &
ParameterBase::description() const noexcept
{
  return _description;
}

const outcome::result<void, std::string>
ParameterBase::validate(const YAML::Node & node) const noexcept
{
  if (!node[_name].IsDefined())
  {
    std::stringstream msg;
    msg << "Error around line " << node.Mark().line << ". ";
    msg << "Parameter " << std::quoted(_name) << " not found in input\n" << node << std::endl;
    return outcome::failure(errorMessage(msg.str()));
  }
  return _type_validater(node[_name]);
}

template <typename T>
Parameter<T>::Parameter(const std::string & name,
                        const std::string & description,
                        const bool required)
  : ParameterBase(name, description, required)
{
  _type_validater = [](const YAML::Node & node) -> const outcome::result<void, std::string>
  {
    std::string type;
    try
    {
      if constexpr (std::is_same_v<T, std::string>)
      {
        type = "std::string";
        node.as<std::string>();
      }
      else if constexpr (std::is_same_v<T, int>)
      {
        type = "int";
        node.as<int>();
      }
      else if constexpr (std::is_same_v<T, unsigned int>)
      {
        type = "unsigned int";
        node.as<unsigned int>();
      }
      else if constexpr (std::is_same_v<T, double>)
      {
        type = "double";
        node.as<double>();
      }
      else if constexpr (std::is_same_v<T, bool>)
      {
        type = "bool";
        node.as<bool>();
      }
      else if constexpr (std::is_same_v<T, std::vector<int>>)
      {
        type = "std::vector<int>";
        node.as<std::vector<int>>();
      }
      return outcome::success();
    }
    catch (const std::exception & e)
    {
      std::stringstream msg;
      msg << "Error on line " << node.Mark().line << ". Unable to parse node contents " << node
          << " as requested type: " << type << std::endl;
      return outcome::failure(errorMessage(msg.str()));
    }
  };
}

template class Parameter<int>;
template class Parameter<bool>;
template class Parameter<double>;
template class Parameter<unsigned int>;
template class Parameter<std::string>;
template class Parameter<std::vector<int>>;
}
