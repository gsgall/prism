#include "gtest/gtest.h"
#include "prism/SpeciesManager.h"
#include "prism/ReactionManager.h"

TEST(ReactionManager, ValidEquations)
{
  auto species_manager = prism::SpeciesManager();
  auto reaction_manager = prism::ReactionManager(species_manager);

  EXPECT_TRUE(reaction_manager.reactionId("Ar + e -> Ar + e"));
  EXPECT_TRUE(reaction_manager.reactionId("Ar + Ar -> Ar + Ar"));
  EXPECT_TRUE(reaction_manager.reactionId("2Ar -> Ar + Ar"));
  EXPECT_TRUE(reaction_manager.reactionId("2Ar -> 2Ar"));
  EXPECT_TRUE(reaction_manager.reactionId("Ar + Ar -> 2Ar"));
}
