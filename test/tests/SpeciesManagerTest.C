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
#include "prism/core/SpeciesManager.h"

TEST(SpeciesManager, CorrectModifiers)
{
  auto manager = prism::SpeciesManager();
  ASSERT_TRUE(manager.speciesId("Ar(alpha)", false));

  EXPECT_EQ(manager.speciesById(0).name(), "Ar");
  EXPECT_EQ(manager.speciesById(0).modifier(), "");
  EXPECT_EQ(manager.speciesById(1).name(), "Ar(alpha)");
  EXPECT_EQ(manager.speciesById(1).modifier(), "(alpha)");
}

TEST(SpeciesManager, ValidSpeciesNames)
{
  auto manager = prism::SpeciesManager();
  EXPECT_TRUE(manager.speciesId("Ar", false));
  EXPECT_TRUE(manager.speciesId("Ar(alpha)", false));
  EXPECT_TRUE(manager.speciesId("Ar2(AAAA)", false));
  EXPECT_TRUE(manager.speciesId("Ar*(A)", false));
  EXPECT_TRUE(manager.speciesId("Ar*(2A*)", false));
  EXPECT_TRUE(manager.speciesId("Ar*(2A*)", false));
  EXPECT_TRUE(manager.speciesId("H3+4(test)", false));
  EXPECT_TRUE(manager.speciesId("Ar2CF4H3+4(test)", false));
  EXPECT_TRUE(manager.speciesId("Ar2CF4H3+4(T)", false));
  EXPECT_TRUE(manager.speciesId("H3-4(*T)", false));
  EXPECT_TRUE(manager.speciesId("e", false));
  EXPECT_TRUE(manager.speciesId("E", false));
  EXPECT_TRUE(manager.speciesId("hnu", false));
}

TEST(SpeciesManager, InvalidSpeciesNames)
{
  auto manager = prism::SpeciesManager();
  EXPECT_FALSE(manager.speciesId("Ar(A", false));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4*T", false));
  EXPECT_FALSE(manager.speciesId("Ar2C(test)F4H3+4(test)", false));
  EXPECT_FALSE(manager.speciesId("Ar2C*F4H3+4(test)", false));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4(test)lkasdf", false));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4ads(test)", false));
  EXPECT_FALSE(manager.speciesId("Z", false));
}
