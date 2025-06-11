#include <any>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace input {


class InputParameters
{
public:
  InputParameters();

  template<typename T>
  void addParam(const std::string & name, const T default_value, const std::string & description);

  template<typename T>
  void addRequiredParam(const std::string & name, const std::string & description);

  bool readInput(YAML::Node & input);

private:

  enum SetMethod{
    USER,
    DEFAULT,
    NOT_SET,
  };

  struct Details{
    SetMethod _how_set = NOT_SET;
    std::string _description = "";
  };
  
  std::unordered_map<std::string, std::pair<Details, std::any>> _params;


};

}
