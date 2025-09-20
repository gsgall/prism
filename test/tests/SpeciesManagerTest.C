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

TEST(SpeciesManager, Modifiers)
{
  auto manager = prism::SpeciesManager();
  auto res0 = manager.speciesId("Ar(alpha)", false);
  ASSERT_TRUE(res0);
  auto res1 = manager.speciesId("Ar+", false);
  ASSERT_TRUE(res1);
  auto res2 = manager.speciesId("Ar+(alpha)", false);
  ASSERT_TRUE(res2);
  auto res3 = manager.speciesId("Ar-(alpha)", false);
  ASSERT_TRUE(res3);
  auto res4 = manager.speciesId("Ar+2(alpha)", false);
  ASSERT_TRUE(res4);
  auto res5 = manager.speciesId("Ar+2*(alpha)", false);
  ASSERT_TRUE(res5);

  EXPECT_EQ(manager.species(res0.value()).name(), "Ar(alpha)");
  EXPECT_EQ(manager.species(res0.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res0.value()).modifier(), "(alpha)");

  EXPECT_EQ(manager.species(res1.value()).name(), "Ar+");
  EXPECT_EQ(manager.species(res1.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res1.value()).modifier(), "");

  EXPECT_EQ(manager.species(res2.value()).name(), "Ar+(alpha)");
  EXPECT_EQ(manager.species(res2.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res2.value()).modifier(), "(alpha)");

  EXPECT_EQ(manager.species(res3.value()).name(), "Ar-(alpha)");
  EXPECT_EQ(manager.species(res3.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res3.value()).modifier(), "(alpha)");

  EXPECT_EQ(manager.species(res4.value()).name(), "Ar+2(alpha)");
  EXPECT_EQ(manager.species(res4.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res4.value()).modifier(), "(alpha)");

  EXPECT_EQ(manager.species(res5.value()).name(), "Ar+2*(alpha)");
  EXPECT_EQ(manager.species(res5.value()).baseName(), "Ar");
  EXPECT_EQ(manager.species(res5.value()).modifier(), "*(alpha)");
}

TEST(SpeciesManager, LaTeXRepresentations)
{
  auto manager = prism::SpeciesManager();
  ASSERT_TRUE(manager.addLatexOverride("Ar(r)", "Ar^{r}"));
  auto res0 = manager.speciesId("Ar", false);
  ASSERT_TRUE(res0);
  auto res1 = manager.speciesId("Ar(alpha)", false);
  ASSERT_TRUE(res1);
  auto res2 = manager.speciesId("Ar2(AAAA)", false);
  ASSERT_TRUE(res2);
  auto res3 = manager.speciesId("Ar*(A)", false);
  ASSERT_TRUE(res3);
  auto res4 = manager.speciesId("Ar+(2A*)", false);
  ASSERT_TRUE(res4);
  auto res5 = manager.speciesId("Ar-(2A*)", false);
  ASSERT_TRUE(res5);
  auto res6 = manager.speciesId("H3+4(test)", false);
  ASSERT_TRUE(res6);
  auto res7 = manager.speciesId("Ar2CF4H3-4(test)", false);
  ASSERT_TRUE(res7);
  auto res8 = manager.speciesId("Ar(r)", false);
  ASSERT_TRUE(res8);
  auto res9 = manager.speciesId("e", false);
  ASSERT_TRUE(res9);
  auto res10 = manager.speciesId("hnu", false);
  ASSERT_TRUE(res10);

  const auto & species = manager.species();
  EXPECT_EQ(species[res0.value()].latex(), "Ar");
  EXPECT_EQ(species[res1.value()].latex(), "Ar\\left(alpha\\right)");
  EXPECT_EQ(species[res2.value()].latex(), "Ar_{2}\\left(AAAA\\right)");
  EXPECT_EQ(species[res3.value()].latex(), "Ar^{*}\\left(A\\right)");
  EXPECT_EQ(species[res4.value()].latex(), "Ar^{+}\\left(2A*\\right)");
  EXPECT_EQ(species[res5.value()].latex(), "Ar^{-}\\left(2A*\\right)");
  EXPECT_EQ(species[res6.value()].latex(), "H_{3}^{4+}\\left(test\\right)");
  EXPECT_EQ(species[res7.value()].latex(), "Ar_{2}CF_{4}H_{3}^{4-}\\left(test\\right)");
  EXPECT_EQ(species[res8.value()].latex(), "Ar^{r}");
  EXPECT_EQ(species[res9.value()].latex(), "e");
  EXPECT_EQ(species[res10.value()].latex(), "h$\\nu$");
}

TEST(SpeciesManager, ValidSpeciesNames)
{
  auto manager = prism::SpeciesManager();
  auto res0 = manager.speciesId("Ar", false);
  auto res1 = manager.speciesId("Ar(alpha)", false);
  auto res2 = manager.speciesId("Ar2(AAAA)", false);
  auto res3 = manager.speciesId("Ar*(A)", false);
  auto res4 = manager.speciesId("Ar*(2A*)", false);
  auto res5 = manager.speciesId("Ar*(2A*)", false);
  auto res6 = manager.speciesId("H3+4(test)", false);
  auto res7 = manager.speciesId("Ar2CF4H3+4(test)", false);
  auto res8 = manager.speciesId("Ar2CF4H3+4(T)", false);
  auto res9 = manager.speciesId("H3-4(*T)", false);
  auto res10 = manager.speciesId("H3-4*(*T)", false);
  auto res11 = manager.speciesId("e", false);
  auto res12 = manager.speciesId("E", false);
  auto res13 = manager.speciesId("hnu", false);
  auto res14 = manager.speciesId("H+", false);
  auto res15 = manager.speciesId("H-", false);

  const auto species = manager.species();

  ASSERT_TRUE(res0);
  ASSERT_TRUE(res1);
  ASSERT_TRUE(res2);
  ASSERT_TRUE(res3);
  ASSERT_TRUE(res4);
  ASSERT_TRUE(res5);
  ASSERT_TRUE(res6);
  ASSERT_TRUE(res7);
  ASSERT_TRUE(res8);
  ASSERT_TRUE(res9);
  ASSERT_TRUE(res10);
  ASSERT_TRUE(res11);
  ASSERT_TRUE(res12);
  ASSERT_TRUE(res13);
  ASSERT_TRUE(res14);
  ASSERT_TRUE(res15);

  EXPECT_EQ(species[res0.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res1.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res2.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res3.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res4.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res5.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res6.value()].chargeNumber(), 4);
  EXPECT_EQ(species[res7.value()].chargeNumber(), 4);
  EXPECT_EQ(species[res8.value()].chargeNumber(), 4);
  EXPECT_EQ(species[res9.value()].chargeNumber(), -4);
  EXPECT_EQ(species[res10.value()].chargeNumber(), -4);
  EXPECT_EQ(species[res11.value()].chargeNumber(), -1);
  EXPECT_EQ(species[res12.value()].chargeNumber(), -1);
  EXPECT_EQ(species[res13.value()].chargeNumber(), 0);
  EXPECT_EQ(species[res14.value()].chargeNumber(), 1);
  EXPECT_EQ(species[res15.value()].chargeNumber(), -1);
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
  EXPECT_FALSE(manager.speciesId("Armodifier", false));
  EXPECT_FALSE(manager.speciesId("Z", false));
  // TODO: make some better rules so that this can be valid syntax
  EXPECT_FALSE(manager.speciesId("Ar^r", false));
}
