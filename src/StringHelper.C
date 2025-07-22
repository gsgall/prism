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
#include "StringHelper.h"

#include <cctype>
#include <cmath>
#include <iostream>
#include <optional>
#include "fmt/core.h"

namespace prism
{
const std::string WHITESPACE = " \n\r\t\f\v";

void
ltrim(std::string & s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  s = (start == std::string::npos) ? "" : s.substr(start);
}

void
rtrim(std::string & s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

void
trim(std::string & s)
{
  ltrim(s);
  rtrim(s);
}

std::vector<std::string>
splitByDelimiter(const std::string & s, const std::string & d)
{
  std::vector<std::string> sub_s;
  auto temp_s = s;
  size_t d_idx;
  std::string left;

  while (temp_s.find(d) != std::string::npos)
  {
    d_idx = temp_s.find(d);
    left = temp_s.substr(0, temp_s.find(d));
    trim(left);
    sub_s.push_back(left);
    temp_s = temp_s.substr(d_idx + d.length(), temp_s.length());
  }

  trim(temp_s);
  sub_s.push_back(temp_s);

  return sub_s;
}

int
findFirstCapital(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (isupper(s[i]))
      return i;

  return -1;
}

int
findFirstSpecial(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (!isalnum(s[i]))
      return i;
  return -1;
}

int
findFirstNonLetter(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (!isalpha(s[i]))
      return i;

  return -1;
}

int
findFirstLetter(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (isalpha(s[i]))
      return i;

  return -1;
}

int
findFirstNonNumber(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (!isdigit(s[i]))
      return i;

  return -1;
}

int
findFirstNumber(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (isdigit(s[i]))
      return i;

  return -1;
}

int
findFirstNonSpecial(const std::string & s)
{
  for (size_t i = 0; i < s.length(); ++i)
    if (isalnum(s[i]))
      return i;

  return -1;
}

std::vector<std::string>
splitByCapital(const std::string & s)
{
  std::vector<std::string> parts;

  int capital_idx = findFirstCapital(s);

  // if there are no capitals then we will just give back an empty container
  if (capital_idx == -1)
    return {s};

  // case for a single character std::string
  if (capital_idx == 0 && s.length() == 1)
    return {s};

  std::string sub_s = s;

  std::vector<int> cut_locations;

  while (capital_idx != -1)
  {
    cut_locations.push_back(capital_idx);
    // lower case the captial and find the next one
    sub_s[capital_idx] = tolower(sub_s[capital_idx]);
    capital_idx = findFirstCapital(sub_s);
  }

  for (size_t i = 0; i < cut_locations.size() - 1; ++i)
    parts.push_back(s.substr(cut_locations[i], cut_locations[i + 1] - cut_locations[i]));

  parts.push_back(s.substr(cut_locations.back(), s.length()));

  return parts;
}

std::string
formatScientific(const double val)
{
  int exponent = 0;

  if (val == 0)
    return "0.00";

  exponent = static_cast<int>(std::floor(std::log10(std::abs(val))));

  double mantissa = val / std::pow(static_cast<double>(10), static_cast<double>(exponent));

  if (exponent > -2 && exponent < 2)
    return fmt::format("{:0.2f}", val);

  return fmt::format("{:.2f}", mantissa) + "$\\times 10^{" + fmt::format("{:d}", exponent) + "}$";
}

bool
balancedSymbols(const std::string & s)
{
  std::stack<char> stack;
  std::vector<char> openers = {'(', '{', '['};
  std::vector<char> closers = {')', '}', ']'};

  for (const auto curr : s)
  {
    for (const auto o : openers)
    {
      if (curr == o)
      {
        stack.push(curr);
        break;
      }
    }

    for (size_t i = 0; i < closers.size(); i++)
    {
      if (curr == closers[i])
      {
        if (stack.empty() || stack.top() != openers[i])
          return false;
        stack.pop();
      }
    }
  }

  return stack.empty();
}

std::optional<std::string>
clearBalancedSymbols(const std::string & s) noexcept
{

  std::stack<char> stack;
  std::vector<char> openers = {'(', '{', '['};
  std::vector<char> closers = {')', '}', ']'};

  std::string filtered;
  for (const auto curr : s)
  {
    for (const auto o : openers)
    {
      if (curr == o)
      {
        stack.push(curr);
        filtered.push_back(curr);
        break;
      }
    }

    if (stack.empty())
      filtered.push_back(curr);

    for (size_t i = 0; i < closers.size(); i++)
    {
      if (curr == closers[i])
      {
        if (stack.empty() || stack.top() != openers[i])
          return std::nullopt;
        filtered.push_back(curr);
        stack.pop();
        break;
      }
    }
  }

  if (!stack.empty())
    return std::nullopt;

  return filtered;
}

std::string
makeGreen(const std::string & s)
{
  return "\033[32m" + s + "\033[0m";
}

std::string
makeRed(const std::string & s)
{
  return "\033[31m" + s + "\033[0m";
}

void
printGreen(const std::string & s)
{
  std::cout << makeGreen(s);
}

void
printRed(const std::string & s)
{
  std::cout << makeRed(s);
}

// std::vector<vector<double>>
// readDataFromFile(const std::string & file,
//                  const std::string & delimiter,
//                  const unsigned int num_columns)
//{
//
//   std::vector<vector<double>> all_data = std::vector<vector<double>>(num_columns);
//   // Create an input file stream
//   std::ifstream data_input(file);
//
//   // Check if the file was opened successfully
//   if (!data_input.is_open())
//     throw InvalidInput("Unable to open data file '" + file + "'");
//
//   std::string line;
//   // Read the file line by line
//   unsigned int line_count = 0;
//   while (getline(data_input, line))
//   {
//     line_count++;
//     if (line.find(delimiter) == std::string::npos)
//     {
//       throw InvalidInput("Unable to find delimiter '" + delimiter + "' on line " +
//                          to_string(line_count) + " of file '" + file + "'");
//     }
//
//     if (line.find(delimiter) == 0)
//     {
//       throw InvalidInput("Delimiter '" + delimiter + "' may not be found on line " +
//                          to_string(line_count) + " of file '" + file + "'\n" +
//                          "The delimieter may also have been found at the beginning of the line");
//     }
//
//     const auto & std::string_data = splitByDelimiter(line, delimiter);
//     if (std::string_data.size() != num_columns)
//     {
//       data_input.close();
//       throw InvalidInput("Line " + to_string(line_count) + " in file '" + file + "' contains " +
//                          to_string((unsigned int)string_data.size()) + " value" +
//                          ((unsigned int)string_data.size() == 1 ? "" : "s") +
//                          " when it should contain " + to_string(num_columns) + " value" +
//                          (num_columns == 1 ? "" : "s"));
//     }
//
//     try
//     {
//       for (unsigned int i = 0; i < num_columns; ++i)
//         all_data[i].push_back(stod(std::string_data[i]));
//     }
//     catch (exception & e)
//     {
//       data_input.close();
//       throw InvalidInput("There was an issue parsing something on line " + to_string(line_count)
//       +
//                          " in file '" + file + "'.");
//     }
//   }
//
//   // Close the file
//   data_input.close();
//
//   return all_data;
// }
}
