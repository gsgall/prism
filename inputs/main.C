//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry *
// https://github.com/NCSU-ComPS-Group/prism * * Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED

// #include <algorithm>
#include <boost/outcome/success_failure.hpp>
#include "inputs/ParamMacros.h"
// #include "inputs/Parameter.h"
#include "inputs/InputParameters.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

int
main()
{
  std::cout << "test" << std::endl;
  const auto node = YAML::LoadFile("simple_argon_rate.yaml");
  std::cout << node << std::endl;

  auto params = inputs::InputParameters();

  auto custom_species = inputs::InputParameters();
  declareRequiredCheckedParam(
      "name",
      "The species name which you are declaring, this must start with a capital letter",
      (
          [](const std::vector<std::string> & names) -> outcome::result<void, std::string>
          {
            for (const std::string & name : names)
            {
              if (std::islower(name.front()))
                return outcome::failure("Custom species names must begin with upper case letters.");
            }
            return outcome::success();
          }),
      custom_species,
      std::vector<std::string>);

  declareRequiredCheckedParam(
      "mass",
      "The mass of the species which you are declaring",
      (
          [](const std::vector<double> & masses) -> outcome::result<void, std::string>
          {
            for (const double mass : masses)
            {
              if (mass <= 0)
                return outcome::failure("Species masses must be non-zero positive numbers.");
            }
            return outcome::success();
          }),
      custom_species,
      std::vector<double>);
  declareRepeatedBlock("custom-species", custom_species, params);

  auto block_base = inputs::InputParameters();

  declareRequiredParam("common", "a parameter both need", block_base, std::vector<std::string>);

  auto block_type1 = block_base;

  declareRequiredParam("param-one", "a param for block type 1", block_type1, int);

  auto block_type2 = block_base;

  declareRequiredParam("param-two", "a param for block type 2", block_type2, std::string);
  declareRepeatedTypedBlock("simple", "one", block_type1, params);
  declareRepeatedTypedBlock("simple", "two", block_type2, params);

  declareRequiredCheckedParam(
      "bibliography",
      "The file that contains the bibtex references for the mechanism",
      [](const std::string & file)
      {
        std::cout << "We can examine the parameter and validate: " << file << std::endl;
        return outcome::success();
        //        return outcome::failure("Invalid file");
      },
      params,
      std::string);

  declareParam("data-path",
               "./",
               "The path to the folder where any input data for the mechanism is placed",
               params,
               std::string);

  declareParam("data-delimiter",
               ",",
               "The tokens which seperate the columns in any data files provided",
               params,
               std::string);

  declareParam("constant-species",
               {},
               "The species which will not evolve over the course of the simulation",
               params,
               std::vector<std::string>);

  std::cout << params.listParameters() << std::endl;

  return EXIT_SUCCESS;
}
