#include <string>

namespace input {

class ParameterBase {
public:
  ParameterBase();
  virtual ~ParameterBase() = default;
  const std::string & description() const {return _description;}
  bool defaultProvided() const {return _default_provided;}
protected:
  std::string _description;
  bool _default_provided;
};

template<typename T>
class Parameter : public ParameterBase
{
  Parameter();

private:
  T _value;
  T _default_value;
};
}
