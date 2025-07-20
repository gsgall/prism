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
#include <string>
#include "gtest/gtest.h"
#include "prism/StringHelper.h"

TEST(StringHelper, ltrim)
{
  std::string unpadded = "tests jal";
  std::string padded = "\n    \t\t tests jal";
  prism::ltrim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, rtrim)
{
  std::string unpadded = "tests jal";
  std::string padded = "tests jal\n    \t\t ";
  prism::rtrim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, trim)
{
  std::string unpadded = "tests jal";
  std::string padded = "     \t\t \n\n    \v tests jal\n\n    \t\t ";
  prism::trim(padded);
  EXPECT_EQ(unpadded, padded);
}

TEST(StringHelper, splitByDelimiter)
{
  std::string unsplit = "Ar + e -> Ar + e";
  std::vector<std::string> split = {"Ar + e", "Ar + e"};

  EXPECT_EQ(split, prism::splitByDelimiter(unsplit, "->"));

  unsplit = "Ar + e";

  split = {"Ar", "e"};

  EXPECT_EQ(split, prism::splitByDelimiter(unsplit, " + "));
}

TEST(StringHelper, findFirstCapital)
{
  std::string test = "adlkfjadHFDKadflk";

  EXPECT_EQ(prism::findFirstCapital(test), 8);

  test = "adskfjh23r8v9832rkjv98013kj";

  EXPECT_EQ(prism::findFirstCapital(test), -1);
}

TEST(StringHelper, findFirstSpecial)
{
  std::string test = "adl(k";

  EXPECT_EQ(prism::findFirstSpecial(test), 3);

  test = "adskfjh23r8v9832rkjv98013kj";

  EXPECT_EQ(prism::findFirstSpecial(test), -1);
}

TEST(StringHelper, findFirstNonLetter)
{
  std::string test = "adl(k";

  EXPECT_EQ(prism::findFirstNonLetter(test), 3);

  test = "adasdfkhjsadipughiujbrvjkb";

  EXPECT_EQ(prism::findFirstNonLetter(test), -1);
}

TEST(StringHelper, findFirstLetter)
{
  std::string test = "12349&q34adl(k";

  EXPECT_EQ(prism::findFirstLetter(test), 6);

  test = "98173298761486700*$%^";

  EXPECT_EQ(prism::findFirstLetter(test), -1);
}

TEST(StringHelper, findFirstNonNumber)
{

  std::string test = "12349&q34adl(k";

  EXPECT_EQ(prism::findFirstNonNumber(test), 5);

  test = "123094871290874321";

  EXPECT_EQ(prism::findFirstNonNumber(test), -1);
}

TEST(StringHelper, findFirstNonSpecial)
{
  std::string test = "  #(*($@(d&*$#))";

  EXPECT_EQ(prism::findFirstNonSpecial(test), 9);

  test = ")*(#@&*@#%&*$#*&)$#&)($#&*())";

  EXPECT_EQ(prism::findFirstNonSpecial(test), -1);
}

TEST(StringHelper, findFirstNumber)
{
  std::string test = "adl1(k";

  EXPECT_EQ(prism::findFirstNumber(test), 3);

  test = "adskfjasdfkljhj";

  EXPECT_EQ(prism::findFirstNumber(test), -1);
}

TEST(StringHelper, splitByCapital)
{
  std::string test = "lkasdfoiwkj";
  std::vector<std::string> result = {};

  EXPECT_TRUE(prism::splitByCapital(test).empty());

  test = "Arh2Hs2JG";
  result = {"Arh2", "Hs2", "J", "G"};

  EXPECT_EQ(prism::splitByCapital(test), result);
}

TEST(StringHelper, formatScientific)
{
  EXPECT_EQ(prism::formatScientific(3.25e14), "3.25$\\times 10^{14}$");
  EXPECT_EQ(prism::formatScientific(3.25e3), "3.25$\\times 10^{3}$");
  EXPECT_EQ(prism::formatScientific(3.25e-3), "3.25$\\times 10^{-3}$");
  EXPECT_EQ(prism::formatScientific(10), "10.00");
  EXPECT_EQ(prism::formatScientific(100), "1.00$\\times 10^{2}$");
  EXPECT_EQ(prism::formatScientific(1000), "1.00$\\times 10^{3}$");
  EXPECT_EQ(prism::formatScientific(0.1), "0.10");
  EXPECT_EQ(prism::formatScientific(0.01), "1.00$\\times 10^{-2}$");
  EXPECT_EQ(prism::formatScientific(0.001), "1.00$\\times 10^{-3}$");
}
