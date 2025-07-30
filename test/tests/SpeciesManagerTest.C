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
#include "prism/SpeciesManager.h"

TEST(SpeciesManager, ValidSpeciesNames)
{
  auto manager = prism::SpeciesManager();
  EXPECT_TRUE(manager.speciesId("Ar"));
  EXPECT_TRUE(manager.speciesId("Ar(alpha)"));
  EXPECT_TRUE(manager.speciesId("Ar2(AAAA)"));
  EXPECT_TRUE(manager.speciesId("Ar*(A)"));
  EXPECT_TRUE(manager.speciesId("Ar*(2A*)"));
  EXPECT_TRUE(manager.speciesId("Ar*(2A*)"));
  EXPECT_TRUE(manager.speciesId("H3+4(test)"));
  EXPECT_TRUE(manager.speciesId("Ar2CF4H3+4(test)"));
  EXPECT_TRUE(manager.speciesId("Ar2CF4H3+4(T)"));
  EXPECT_TRUE(manager.speciesId("H3-4(*T)"));
  EXPECT_TRUE(manager.speciesId("e"));
  EXPECT_TRUE(manager.speciesId("E"));
  EXPECT_TRUE(manager.speciesId("hnu"));
}

TEST(SpeciesManager, InvalidSpeciesNames)
{
  auto manager = prism::SpeciesManager();
  EXPECT_FALSE(manager.speciesId("Ar(A"));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4*T"));
  EXPECT_FALSE(manager.speciesId("Ar2C(test)F4H3+4(test)"));
  EXPECT_FALSE(manager.speciesId("Ar2C*F4H3+4(test)"));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4(test)lkasdf"));
  EXPECT_FALSE(manager.speciesId("Ar2CF4H3+4ads(test)"));
  EXPECT_FALSE(manager.speciesId("Z"));
}
