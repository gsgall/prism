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
#include "gtest/gtest.h"
#include "prism/core/NetworkParser.h"

TEST(NetworkParserTest, ConstantSpecies)
{
  std::istringstream input(R"(
bibliography: inputs/works.bib
constant-species: [Ar]

rate-based:
  - reaction: Ar + e -> Ar(r) + e
    type: ConstantRateReaction
    rate: 1
    references: [lymberopoulos1993fluid]
)");

  auto np = prism::NetworkParser();

  const std::string errors = np.parseNetwork(input);
  ASSERT_TRUE(errors.empty()) << errors;

  const std::vector<prism::SpeciesId> & constant_ids = np.constantSpeciesIds();
  const std::vector<prism::SpeciesId> & transient_ids = np.transientSpeciesIds();
  const std::vector<prism::Species> & species = np.species();

  ASSERT_EQ(constant_ids.size(), static_cast<size_t>(1));
  EXPECT_EQ(constant_ids.front(), static_cast<prism::SpeciesId>(0));
  EXPECT_EQ(species[constant_ids.front()].name(), "Ar");

  ASSERT_EQ(transient_ids.size(), static_cast<size_t>(2));
  EXPECT_EQ(transient_ids.front(), static_cast<prism::SpeciesId>(1));
  EXPECT_EQ(species[transient_ids.front()].name(), "e");
  EXPECT_EQ(transient_ids.back(), static_cast<prism::SpeciesId>(2));
  EXPECT_EQ(species[transient_ids.back()].name(), "Ar(r)");
}
