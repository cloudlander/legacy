/* File: errors.h
 */

#ifndef _H_errors
#define _H_errors

#include <string>
#include "location.h"
class ReportError
{
 public:

  // Errors used by preprocessor
  static void UntermComment();
  static void InvalidDirective(int linenum);


  // Errors used by scanner
  static void LongIdentifier(yyltype *loc, const char *ident);
  static void UntermString(yyltype *loc, const char *str);
  static void UnrecogChar(yyltype *loc, char ch);


  // Generic method to report a printf-style error message
  static void Formatted(yyltype *loc, const char *format, ...);

  static void RuntimeError(const char *format, ...);

  // Returns number of error messages printed
  static int NumErrors() { return numErrors; }
  
 private:

  static void UnderlineErrorInLine(const char *line, yyltype *pos);
  static void OutputError(yyltype *loc, const char *msg);
  static int numErrors;
  
};

#endif
