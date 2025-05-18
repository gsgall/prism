#include "Registrar.h"

Registrar * Registrar::_instance = nullptr;

Registrar &
Registrar::instance()
{
  // Create the _instance if it does not exist
  if (_instance == nullptr)
    _instance = new Registrar();

  return *_instance;
}
