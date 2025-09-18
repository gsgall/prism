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
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace prism
{
/**
 * Trim the white space from left end of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void ltrim(std::string & s);

/**
 * Trim the white space from right end of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void rtrim(std::string & s);

/**
 * Trim the white space from both ends of a std::string.
 * This function will modify the value of s
 * @param s the std::string that will be modified
 */
void trim(std::string & s);

/**
 * Break a given std::string into pieces based on the provided delimieter
 * This will not modified the value of std::string s
 * Ex: "A + B -> C + D" with delimiter " -> " will return {"A + B", "C + D"}
 * @param s the std::string that will be split into pieces
 * @param d the delimiter that will be used to split the std::string
 * @returns A std::vector of std::strings which are the parts of the std::strings seperated by the
 * delimiter
 */
std::vector<std::string> splitByDelimiter(const std::string & s, const std::string & d);

/**
 * Find the first capital letter in a std::string
 * @param s the std::string to search
 * @returns the index of the first capital letter if
 *          no capital letter -1 is returned
 */
int findFirstCapital(const std::string & s);

/**
 * Find the first special character in a std::string
 * @param s the std::string to search
 * @returns the index of the first special character if
 *          no special character is found -1 is returned
 */
int findFirstSpecial(const std::string & s);

/**
 * Find the first non letter in a std::string
 * @param s the std::string to search
 * @returns the index of the non letter if
 *          no non letter is found -1 is returned
 */
int findFirstNonLetter(const std::string & s);

/**
 * Find the first letter in a std::string
 * @param s the std::string to search
 * @returns the index of the letter if
 *          no letter is found -1 is returned
 */
int findFirstLetter(const std::string & s);

/**
 * Find the first non number in a std::string
 * @param s the std::string to search
 * @returns the index of the non number if
 *          no non number is found -1 is returned
 */
int findFirstNonNumber(const std::string & s);

/**
 * Find the first number in a std::string
 * @param s the std::string to search
 * @returns the index of the non number if
 *          no number is found -1 is returned
 */
int findFirstNumber(const std::string & s);

/**
 * @param s the std::string to search
 * Find the first non special character in a std::string
 * @returns the index of the non special character if
 *          no non special character is found -1 is returned
 */
int findFirstNonSpecial(const std::string & s);

/**
 * Split the std::string by the capital letters
 * @param s the std::string to split
 * @returns a std::vector of substd::strings which has been split by captial letters
 *          if no capitals are found it will be a std::vector with one element
 */
std::vector<std::string> splitByCapital(const std::string & s);

std::string formatScientific(const double val);

/**
 * Method add the green escape color to the std::string and prints
 * to standard output
 * @param s the std::string to print in green
 */
void printGreen(const std::string & s);

/**
 * Adds the red escape color to the front of a std::string and
 * then adds the default color escape color the end of it so
 * no other text color is changed
 * @param s the std::string to add the characters to
 */
std::string makeRed(const std::string & s);

/**
 * Checks to see if a string potentially containing parenthesis, or braces is valid or not.
 * @param s the string potentially containing the symbols that need to be balanced
 * @returns true if there is a balanced set of parenthesis or if the string contains none of these
 * special characters as well false if not
 */
bool balancedSymbols(const std::string & s);

/**
 * Removes the content in a string that contains balanced symbol sets
 * #Ex: {[](this is something)} -> {[]()}
 *
 * @param s the string to remove the content from
 * @returns a result object which contains the string with the content within the symbols removed on
 * success and an error if there is a mismatched set of symbols
 */
std::optional<std::string> clearBalancedSymbols(const std::string & s) noexcept;

/**
 * This will take a string that contains a balanced set of parenthesis () or any other valid
 * combination of this as defined by the rules in balancedSymbols and it will produce the proper
 * latex version of this so for a single set of paranethsis it will end up being \left(\right)
 * TODO: Do some error handling on this for the case where an unbalanced set is provided this shoudl
 * not be a problem for prism since we validate that this is true for all species names but it might
 * be a general feature that would be nice
 */
std::string latexParenthesis(const std::string & s) noexcept;
/**
 * Colleces data from files which have columns of data seperated by a
 * delimiter
 * @param file the file which contains the data
 * @param delimiter the string that seperates the columns
 * @param num_columns the number of columns allowed in the file
 */
std::vector<std::vector<double>> readDataFromFile(const std::string & file,
                                                  const std::string & delimiter,
                                                  const unsigned int num_columns);
}
