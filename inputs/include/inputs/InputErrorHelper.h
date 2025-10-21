#pragma once
#include <string>

/**
 * Appends the errors message from a previous result object and a new error message
 * The new error message will take the form
 * file2:function2:line2: message2
 * file1:function1:line1: message1
 * @param result the previous result object
 * @param message a string which contains a new error message to add additional context
 * @returns a new string with the previous error string appended below the new message
 */
#define appendErrorMessage(result, message)                                                        \
  errorWithContext(message, __FILE__, __LINE__, __FUNCTION__) + "\n" + result.error()

/**
 * Adds additional context to the error message it will add
 * the file, function, and line number on which the error occured
 * The new error message will take the form
 * file:function:line: message
 * @param message a string which contains a new error message to add additional context
 * @returns a new string with the previous error string appended below the new message
 */
#define errorMessage(message) errorWithContext(message, __FILE__, __LINE__, __FUNCTION__)

namespace inputs
{
const std::string
errorWithContext(const std::string & message, const char * file, int line, const char * function);
}
