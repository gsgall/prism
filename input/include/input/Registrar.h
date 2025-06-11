#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <sstream>

#define combineNames1(X, Y) X##Y
#define combineNames(X, Y) combineNames1(X, Y)

#define registerApp(app)                                       \
  static char combineNames(app_dummy_for_##app, __COUNTER__) = \
      Registrar::addApp(#app, __FILE__, __LINE__)

#define registerInputBlock(app, block_name)                                       \
  static char combineNames(top_level_block_dummy_for_##block_name, __COUNTER__) = \
      Registrar::addBlock(#app, #block_name, __FILE__, __LINE__)



class Registrar
{
public:
  /** Getter for the singleton instance */
  static Registrar & instance();

  ///@{ Don't allow creation through copy/move construction or assignment
  Registrar(Registrar const &) = delete;
  Registrar & operator=(Registrar const &) = delete;

  Registrar(Registrar &&) = delete;
  Registrar & operator=(Registrar &&) = delete;
  ///@}

///
//  template <typename T>
//  static void addBlock(std::string app, std::string block_name)
//  {
//    std::cout << app << "  " << block_name << std::endl;
//  }
  static char addApp(const std::string & app, const std::string & file, const unsigned int line)
  {
    if (instance()._apps.count(app) != 0)
    {
      std::cerr << "Error in " << file << " on line " << line 
                << "\nApp '" << app << "' is already registered" << std::endl;
      exit(EXIT_FAILURE);
    }
    instance()._apps.insert(app); 
    instance()._app_to_blocks.try_emplace(app);
    return 0;
  }

  static char addBlock(const std::string & app, const std::string & block_name, const std::string & file, const unsigned int line)
  {
    const auto & app_res = checkAppRegistration(app); 
    std::stringstream error_msg; 
    error_msg << "Error in " << file << " on line " << line << "\n"; 
    if (app_res.first == false)
    {
      std::cerr << error_msg.str() << "Could not register block '" << block_name << "' because\nthe app '" 
                << app << "' is not registered." << std::endl;
      exit(EXIT_FAILURE);
    }

    const auto & block_result = checkBlockRegistration(app, block_name);
    if (block_result.first == true)
    {
      std::cerr << error_msg.str() << "Could not register block '" << block_name << "' because\n" 
                << block_result.second << std::endl;
      exit(EXIT_FAILURE);
    }

    auto  app_it = instance()._app_to_blocks.find(app);
    app_it->second.try_emplace(block_name);

    return 0;
  }
 
  static std::pair<bool, std::string> checkAppRegistration(std::string app)
  {
    std::stringstream msg; 
    msg << "App '" << app << "' ";
    if (instance()._apps.count(app) == 0)
    {
      msg << "is not registered.";
      return std::make_pair(false, msg.str());
    }
    msg << "is registered.";
    return std::make_pair(true, msg.str());
  }

  static std::pair<bool, std::string> checkBlockRegistration(std::string app, std::string block_name)
  {
    std::stringstream msg;
    msg << "Block '" << block_name << "' ";

    const auto & result = checkAppRegistration(app);
    if (result.first == false)
    {
      msg << "is not registered because\n" << result.second;
      return std::make_pair(false, msg.str());
    }

    const auto app_it = instance()._app_to_blocks.find(app);
    const auto block_it = app_it->second.find(block_name); 
    if (block_it == app_it->second.end()){
      msg << "is not registered with the app '" << app << "'";
      return std::make_pair(false, msg.str());
    }

    msg << "is registered with the app '" << app << "'";
    return std::make_pair(true, msg.str());
  }

private:

  static Registrar * _instance;

  Registrar(){};

  std::unordered_set<std::string> _apps;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _app_to_blocks;

};
