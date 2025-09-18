#include "gtest/gtest.h"
#include <memory>
#include "inputs/InputParameters.h"
#include "prism/core/SpeciesManager.h"
#include "prism/core/ReactionManager.h"
#include "prism/reactions/ConstantRateReaction.h"

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

  params->setParam<std::string>("reaction", "Ar + Ar -> Ar + Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(1));

  params->setParam<std::string>("reaction", "2Ar -> Ar + Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(2));

  params->setParam<std::string>("reaction", "2Ar -> 2Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(3));

  params->setParam<std::string>("reaction", "Ar + Ar -> 2Ar");
  res = reaction_manager.reactionId(params, true);
  ASSERT_TRUE(res) << res.error();
  EXPECT_EQ(res.value(), static_cast<prism::ReactionId>(4));
}
