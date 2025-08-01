//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism *
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include <boost/outcome/success_failure.hpp>

#include "inputs/InputParameters.h"
#include <iostream>
#include "inputs/ParamMacros.h"
#include "yaml-cpp/yaml.h"
#include <vector>
int
main()
{
  YAML::Node node;
  node["test"] = YAML::Load("[-1]");

  node.as<std::vector<unsigned int>>();
  //  auto params = inputs::InputParameters();
  //
  //  addRequiredParam("test", "this does something", params, unsigned int);
  //  std::cout << params.getParam<unsigned int>("test") << std::endl;
  //  params.addParam<int>("test", 0, "this does something");
  //  params.addRequiredParam<unsigned int>("another", "this does something else");
  //  params.addParam<unsigned int>("test", 0, "this does something");

  return EXIT_SUCCESS;
}
