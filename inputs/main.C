//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry *
// https://github.com/NCSU-ComPS-Group/prism * * Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED

#include <algorithm>
#include <boost/outcome/success_failure.hpp>

#include "inputs/Parameter.h"
#include "inputs/InputParameters.h"
#include "yaml-cpp/yaml.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/parse.h>
#include <any>
#include "inputs/ParamMacros.h"

int
main()
{
  auto params = inputs::InputParameters();

  declareRequiredParam("map", "a map", params, std::unordered_map<std::string, int>);

  std::istringstream input("map: {A: 1, A: 2}");

  const auto errors = params.parseInput(input);
  std::cout << errors << std::endl;

  return EXIT_SUCCESS;
}
