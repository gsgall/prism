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
#include <memory>
#include "prism/PrismTypes.h"
#include "prism/RateReactionBase.h"
#include "prism/NetworkParser.h"

TEST(NetworkParserTest, ConsistentIndicies)
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
  const std::vector<std::unique_ptr<prism::RateReactionBase>> & reactions = np.rateReactions();

  ASSERT_EQ(reactions.size(), static_cast<size_t>(1));
  EXPECT_EQ(reactions.front()->equation(), "Ar + e -> Ar(r) + e");
  EXPECT_EQ(reactions.front()->reactantData().size(), static_cast<size_t>(2));
  EXPECT_EQ(reactions.front()->reactantData().front().id, static_cast<prism::SpeciesId>(0));
  EXPECT_EQ(reactions.front()->reactantData().front().occurances, static_cast<unsigned int>(1));
  EXPECT_EQ(reactions.front()->reactantData().back().id, static_cast<prism::SpeciesId>(1));
  EXPECT_EQ(reactions.front()->reactantData().back().occurances, static_cast<unsigned int>(1));

  // While the reaction does have the reactantData in the opposite order we so sort them when we
  // parse everything so that all of the ids are in sorted order
  EXPECT_EQ(reactions.front()->productData().size(), static_cast<size_t>(2));
  EXPECT_EQ(reactions.front()->productData().front().id, static_cast<prism::SpeciesId>(1));
  EXPECT_EQ(reactions.front()->productData().front().occurances, static_cast<unsigned int>(1));
  EXPECT_EQ(reactions.front()->productData().back().id, static_cast<prism::SpeciesId>(2));
  EXPECT_EQ(reactions.front()->productData().back().occurances, static_cast<unsigned int>(1));

  ASSERT_EQ(constant_ids.size(), static_cast<size_t>(1));
  EXPECT_EQ(constant_ids.front(), static_cast<prism::SpeciesId>(0));
  const auto ar = species[constant_ids.front()];

  const std::vector<prism::ReactionData> & ar_rxn_data = ar.rateReactionData();
  EXPECT_EQ(ar.name(), "Ar");
  EXPECT_EQ(ar.id(), constant_ids.front());
  EXPECT_EQ(ar_rxn_data.front().id, static_cast<prism::SpeciesId>(0));
  EXPECT_EQ(ar_rxn_data.front().stoic_coeff, -1);

  const auto e = species[transient_ids.front()];
  const std::vector<prism::ReactionData> & e_rxn_data = e.rateReactionData();
  const auto ar_v = species[transient_ids.back()];
  const std::vector<prism::ReactionData> & ar_v_rxn_data = ar_v.rateReactionData();

  ASSERT_EQ(transient_ids.size(), static_cast<size_t>(2));
  EXPECT_EQ(transient_ids.front(), static_cast<prism::SpeciesId>(1));
  EXPECT_EQ(transient_ids.back(), static_cast<prism::SpeciesId>(2));

  EXPECT_EQ(e.name(), "e");
  EXPECT_EQ(e.id(), transient_ids.front());
  EXPECT_EQ(e_rxn_data.front().id, static_cast<prism::SpeciesId>(0));
  EXPECT_EQ(e_rxn_data.front().stoic_coeff, 0);

  EXPECT_EQ(ar_v.name(), "Ar(r)");
  EXPECT_EQ(ar_v.id(), transient_ids.back());
  EXPECT_EQ(ar_v_rxn_data.front().id, static_cast<prism::SpeciesId>(0));
  EXPECT_EQ(ar_v_rxn_data.front().stoic_coeff, 1);
}
