//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism * * Licensed under MIT, please see LICENSE for details
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
#include <stdexcept>
#include <vector>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/parse.h>
#include <any>
#include "inputs/ParamMacros.h"

int
main()
{

  YAML::Node nodes = YAML::LoadFile("test.yaml");

  //  for (const auto & node : nodes["latex-overrides"])
  //    std::cout << node << std::endl << std::endl;
  //  std::cout << nodes["latex-overrides"] << std::endl;

  auto params = inputs::InputParameters();

  addRequiredCheckedParam(
      "bibliography",
      "The file that contains the bibtex references for the mechanism",
      [](const std::string & file)
      {
        std::cout << "We can examine the parameter and validate: " << file << std::endl
                  << std::endl
                  << std::endl;
        return outcome::success();
      },
      params,
      std::string);

  addParam("data-path",
           "./",
           "The path to the folder where any input data for the mechanism is placed",
           params,
           std::string);

  addParam("data-delimiter",
           ",",
           "The tokens which seperate the columns in any data files provided",
           params,
           std::string);

  addParam("constant-species",
           {},
           "A list of species in the mechanism which are in the reaction mechanism but do not "
           "evolve over time",
           params,
           std::vector<std::string>);

  auto override = inputs::InputParameters();
  override.addRequiredParam<std::vector<std::string>>("species",
                                                      "the species that needs a latex override");
  override.addRequiredParam<std::vector<std::string>>("latex",
                                                      "the latex representation for the species");
  params.addRepeatedSubBlock("latex-overrides", override);

  auto lumped = inputs::InputParameters();
  lumped.addRequiredParam<std::string>("lumped", "the lumped state for the species");
  lumped.addRequiredParam<std::vector<std::string>>(
      "actual", "the actual states which are being lumped together");
  params.addRepeatedSubBlock("lumped-species", lumped);

  params.readFromNodes(nodes);

  std::cout << "Printing the inputs " << std::endl << std::endl;

  std::cout << "bibliography: " << params.getParam<std::string>("bibliography") << std::endl;
  std::cout << "data-path: " << params.getParam<std::string>("data-path") << std::endl;
  std::cout << "data-delimiter: " << params.getParam<std::string>("data-delimiter") << std::endl;

  std::cout << "constant-species: " << std::endl;
  for (const auto & cs : params.getParam<std::vector<std::string>>("constant-species"))
    std::cout << "  " << cs << std::endl;

  std::cout << "latex-overrides: " << std::endl;
  for (const auto & op : params.subBlocks("latex-overrides"))
  {
    std::cout << "  species: ";
    for (const auto & sp : op->getParam<std::vector<std::string>>("species"))
    {
      std::cout << sp << " ";
    }
    std::cout << std::endl;
    std::cout << "  latex: ";
    for (const auto & sp : op->getParam<std::vector<std::string>>("latex"))
    {
      std::cout << sp << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "lumped-species: " << std::endl;
  for (const auto & ls : params.subBlocks("lumped-species"))
  {
    std::cout << "  lumped: " << ls->getParam<std::string>("lumped") << std::endl;
    std::cout << "  actual: ";
    for (const auto & a : ls->getParam<std::vector<std::string>>("actual"))
    {
      std::cout << a << " ";
    }
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
