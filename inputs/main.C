//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism *
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED
//*
#include <algorithm>
#include <boost/outcome/success_failure.hpp>

#include "inputs/Parameter.h"
#include "inputs/InputParameters.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <any>
int
main()
{
  std::unique_ptr<inputs::ParameterBase> param = std::make_unique<inputs::Parameter<unsigned int>>(
      "test",
      1,
      "this parameter does something",
      [](const unsigned int &) { return outcome::success(); });

  const auto res = param->set((unsigned int)(1));

  if (!res)
    std::cout << res.error() << std::endl;

  const auto res2 = param->value();

  if (!res2)
    std::cout << res2.error() << std::endl;

  //  std::cout << std::any_cast<int>(param2->value()) << std::endl;
  //  YAML::Node nodes = YAML::LoadFile("test.yaml");
  //
  //  //  for (const auto & node : nodes["latex-overrides"])
  //  //    std::cout << node << std::endl << std::endl;
  //  //  std::cout << nodes["latex-overrides"] << std::endl;
  //
  //  auto params = inputs::InputParameters();
  //
  //  addRequiredParam("species", "", params, std::vector<std::string>);
  //    addRequiredParam("bibliography",
  //                     "The file that contains the bibtex references for the
  //                     mechanism", params, std::string);
  //
  //  addParam("data-path",
  //           "./",
  //           "The path to the folder where any input data for the mechanism is
  //           placed", params, std::string);
  //
  //  addParam("data-delimiter",
  //           ",",
  //           "The tokens which seperate the columns in any data files
  //           provided", params, std::string);
  //
  //  addParam("constant-species",
  //           {},
  //           "A list of species in the mechanism which are in the reaction
  //           mechanism but do not " "evolve over time", params,
  //           std::vector<std::string>);

  //  params.readFromNodes(nodes["latex-overrides"]);

  return EXIT_SUCCESS;
}
