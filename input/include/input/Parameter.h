#include <string>
#include <optional>
#include <typeindex>
#include "boost/outcome.hpp"
#include "boost/outcome/result.hpp"

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
namespace YAML
{
class Node;
}

namespace input
{

template <typename T>
std::string typeName();

template <typename T>
std::string parseErrorMessage(const std::string & name, const YAML::Node & node, const bool vector);

std::string missingParamErrorMessage(const std::string & name);

class ParameterBase
{
public:
  virtual ~ParameterBase() = default;
  ParameterBase(const std::string name, const std::string description, const bool required);

  const std::string & description() const { return _description; }
  const std::string & name() const { return _name; }
  virtual const std::type_index type() const = 0;

protected:
  const std::string _name;
  const std::string _description;
  const bool _required;
};

template <typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(const std::string name, const std::string description);

  Parameter(const std::string name, const T default_value, const std::string description);

  outcome::result<void, std::string> parseInput(const YAML::Node & node);
  const std::optional<T> value() const { return _value; }
  virtual const std::type_index type() const override { return typeid(T); }

private:
  std::optional<T> _value;
};

template <typename T>
class Parameter<std::vector<T>> : public ParameterBase
{
public:
  Parameter(const std::string name, const std::string description);
  Parameter(const std::string name,
            const std::vector<T> default_value,
            const std::string description);

  outcome::result<void, std::string> parseInput(const YAML::Node & node);
  const std::optional<std::vector<T>> value() const { return _value; }

  virtual const std::type_index type() const override { return typeid(std::vector<T>); }

private:
  std::optional<std::vector<T>> _value;
};
}
