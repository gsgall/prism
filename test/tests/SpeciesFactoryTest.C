
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
#include "prism/SpeciesFactory.h"

TEST(SpeciesFactory, ValidSpeciesNames)
{
  auto factory = prism::SpeciesFactory();
  EXPECT_TRUE(factory.speciesId("Ar"));
  EXPECT_TRUE(factory.speciesId("Ar(alpha)"));
  EXPECT_TRUE(factory.speciesId("Ar2(AAAA)"));
  EXPECT_TRUE(factory.speciesId("Ar*(A)"));
  EXPECT_TRUE(factory.speciesId("Ar*(2A*)"));
  EXPECT_TRUE(factory.speciesId("Ar*(2A*)"));
  EXPECT_TRUE(factory.speciesId("H3+4(test)"));
  EXPECT_TRUE(factory.speciesId("Ar2CF4H3+4(test)"));
  EXPECT_TRUE(factory.speciesId("Ar2CF4H3+4(T)"));
  EXPECT_TRUE(factory.speciesId("H3-4(*T)"));
}

TEST(SpeciesFactory, InvalidSpeciesNames)
{
  auto factory = prism::SpeciesFactory();
  EXPECT_FALSE(factory.speciesId("Ar(A"));
  EXPECT_FALSE(factory.speciesId("Ar2CF4H3+4*T"));
  EXPECT_FALSE(factory.speciesId("Ar2C(test)F4H3+4(test)"));
  EXPECT_FALSE(factory.speciesId("Ar2C*F4H3+4(test)"));
  EXPECT_FALSE(factory.speciesId("Ar2CF4H3+4(test)lkasdf"));
  EXPECT_FALSE(factory.speciesId("Ar2CF4H3+4ads(test)"));
}
