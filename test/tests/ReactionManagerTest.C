#include "gtest/gtest.h"
#include <memory>
#include "inputs/InputParameters.h"
#include "prism/SpeciesManager.h"
#include "prism/ReactionManager.h"
#include "prism/ConstantRateReaction.h"

TEST(ReactionManager, ValidEquations)
{
  auto species_manager = prism::SpeciesManager();
  auto reaction_manager = prism::ReactionManager(species_manager);

  auto params =
      std::make_unique<inputs::InputParameters>(prism::ConstantRateReaction::validParams());
  params->addRequiredParam<std::string>("type", "The reaction type");
  params->setParam<std::string>("type", "ConstantRateReaction");
  params->setParam<double>("rate", 1);
  params->setParam<std::vector<std::string>>("references", {});

  params->setParam<std::string>("reaction", "Ar + e -> Ar + e");
  auto res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(0));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "Ar + Ar -> Ar + Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(1));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "2Ar -> Ar + Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(2));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "2Ar -> 2Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(3));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "Ar + Ar -> 2Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(4));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "Ar+ + e -> Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(5));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());

  params->setParam<std::string>("reaction", "Ar- -> Ar + e");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(6));
  EXPECT_EQ(res.value(), reaction_manager.rateReaction(res.value())->id());
}

TEST(ReactionManager, ReactionStoicCoeffs)
{

  auto species_manager = prism::SpeciesManager();
  auto reaction_manager = prism::ReactionManager(species_manager);

  auto params =
      std::make_unique<inputs::InputParameters>(prism::ConstantRateReaction::validParams());
  params->addRequiredParam<std::string>("type", "The reaction type");
  params->setParam<std::string>("type", "ConstantRateReaction");
  params->setParam<double>("rate", 1);
  params->setParam<std::vector<std::string>>("references", {});

  params->setParam<std::string>("reaction", "Ar + e -> Ar + e");
  auto res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();

  params->setParam<std::string>("reaction", "Ar+ + e -> Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();

  params->setParam<std::string>("reaction", "Ar- -> Ar + e");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();

  const auto & rxns = reaction_manager.rateReactions();
  const auto & species = species_manager.species();

  for (const auto & rxn : rxns)
  {
    for (const prism::SpeciesData & data : rxn->reactantData())
    {
      auto it = std::find_if(species[data.id].rateReactionData().begin(),
                             species[data.id].rateReactionData().end(),
                             [&rxn](const prism::ReactionData & rxn_data)
                             { return rxn_data.id == rxn->id(); });

      EXPECT_EQ(data.stoic_coeff, it->stoic_coeff);
    }
    for (const prism::SpeciesData & data : rxn->productData())
    {
      auto it = std::find_if(species[data.id].rateReactionData().begin(),
                             species[data.id].rateReactionData().end(),
                             [&rxn](const prism::ReactionData & rxn_data)
                             { return rxn_data.id == rxn->id(); });

      EXPECT_EQ(data.stoic_coeff, it->stoic_coeff);
    }
  }
}
