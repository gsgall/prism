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
  declareRequiredParam("string-param", "a string parameter", params, std::string);

  std::istringstream input_stream("other-param: something");

  const std::string errors = params.parseInput(input_stream);
  if (!errors.empty())
    std::cout << errors;
  return 0;
  //  auto params = inputs::InputParameters();
  //
  //  declareRequiredCheckedParam(
  //      "bibliography",
  //      "The file that contains the bibtex references for the mechanism",
  //      [](const std::string & file)
  //      {
  //        std::cout << "We can examine the parameter and validate: " << file << std::endl;
  //        return outcome::success();
  //        //        return outcome::failure("Invalid file");
  //      },
  //      params,
  //      std::string);
  //
  //  declareParam("data-path",
  //               "./",
  //               "The path to the folder where any input data for the mechanism is placed",
  //               params,
  //               std::string);
  //
  //  declareParam("data-delimiter",
  //               ",",
  //               "The tokens which seperate the columns in any data files provided",
  //               params,
  //               std::string);
  //
  //  declareParam("constant-species",
  //               {},
  //               "A list of species in the mechanism which are in the reaction mechanism but do
  //               not " "evolve over time", params, std::vector<std::string>);
  //
  //  auto override = inputs::InputParameters();
  //  override.addRequiredParam<std::vector<std::string>>("species",
  //                                                      "the species that needs a latex
  //                                                      override");
  //  override.addRequiredParam<std::vector<std::string>>("latex",
  //                                                      "the latex representation for the
  //                                                      species");
  //  params.addRepeatedBlock("latex-overrides", override);
  //
  //  auto lumped = inputs::InputParameters();
  //  lumped.addRequiredParam<std::string>("lumped", "the lumped state for the species");
  //  lumped.addRequiredParam<std::vector<std::string>>(
  //      "actual", "the actual states which are being lumped together");
  //
  //  declareRequiredRepeatedBlock("lumped-species", lumped, params);
  //
  //  auto rxn_base = inputs::InputParameters();
  //  rxn_base.addRequiredParam<std::string>("reaction", "The chemical reaction to be parsed.");
  //
  //  rxn_base.addParam<std::vector<std::string>>(
  //      "notes", {}, "Any notes for things that are important to know about the reaction.");
  //
  //  rxn_base.addRequiredParam<std::vector<std::string>>(
  //      "references", "A list of bibtex keys for the sources where the reaction data came from.");
  //
  //  auto rxn_constant = rxn_base;
  //
  //  rxn_constant.addRequiredParam<double>("rate",
  //                                        "The constant rate that the reaction is occuring at");
  //
  //  auto rxn_arr = rxn_base;
  //
  //  rxn_arr.addRequiredParam<std::unordered_map<std::string, double>>(
  //      "params", "Coefficients for an arrhenius reaction type equation");
  //
  //  declareRepeatedTypedBlock("rate-based", "constant", rxn_constant, params);
  //  declareRepeatedTypedBlock("rate-based", "arrhenius", rxn_arr, params);
  //
  //  const auto errors = params.parseInput("test.yaml");
  //  if (!errors.empty())
  //  {
  //    std::cout << errors << std::endl;
  //    return EXIT_FAILURE;
  //  }
  //  std::cout << "Success!" << std::endl;
  //
  //  // params.addRepeatedSubBlock("latex-overrides", override);
  //  //   std::cout << "Printing the inputs " << std::endl << std::endl;
  //  //   std::cout << "bibliography: " << params.getParam<std::string>("bibliography") <<
  //  //   std::endl; std::cout << "data-path: " << params.getParam<std::string>("data-path") <<
  //  //   std::endl; std::cout << "data-delimiter: " <<
  //  //   params.getParam<std::string>("data-delimiter") << std::endl;
  //  //
  //  //   std::cout << "constant-species: " << std::endl;
  //  //   for (const auto & cs : params.getParam<std::vector<std::string>>("constant-species"))
  //  //     std::cout << "  " << cs << std::endl;
  //  //
  //  //   std::cout << "latex-overrides: " << std::endl;
  //  //   for (const auto & op : params.subBlocks("latex-overrides"))
  //  //   {
  //  //     std::cout << "  species: ";
  //  //     for (const auto & sp : op->getParam<std::vector<std::string>>("species"))
  //  //     {
  //  //       std::cout << sp << " ";
  //  //     }
  //  //     std::cout << std::endl;
  //  //     std::cout << "  latex: ";
  //  //     for (const auto & sp : op->getParam<std::vector<std::string>>("latex"))
  //  //     {
  //  //       std::cout << sp << " ";
  //  //     }
  //  //     std::cout << std::endl;
  //  //   }
  //  //
  //  //   std::cout << "lumped-species: " << std::endl;
  //  //   for (const auto & ls : params.subBlocks("lumped-species"))
  //  //   {
  //  //     std::cout << "  lumped: " << ls->getParam<std::string>("lumped") << std::endl;
  //  //     std::cout << "  actual: ";
  //  //     for (const auto & a : ls->getParam<std::vector<std::string>>("actual"))
  //  //     {
  //  //       std::cout << a << " ";
  //  //     }
  //  //     std::cout << std::endl;
  //  //   }
  //  //
  //  return EXIT_SUCCESS;
  //

  return EXIT_SUCCESS;
}
