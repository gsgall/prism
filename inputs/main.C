//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdlib.h>

#include "yaml-cpp/yaml.h"
#include "inputs/Parameter.h"
int
main()
{
  YAML::Node node;
  node["test"] = YAML::Load("[1, 2, 3, 4]");

  auto param =
      std::make_unique<inputs::ParameterBase>(inputs::Parameter<std::string>("test", "", true));

  auto res = param->validate(node);
  if (!res)
    std::cout << res.error() << std::endl;
  else
    std::cout << "Parsing Success!" << std::endl;

  return EXIT_SUCCESS;
}
