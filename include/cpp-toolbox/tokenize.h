
#pragma once

#include <string>
#include <vector>

namespace toolbox {
namespace strutil {

typedef std::vector<std::string> str_vec_t;

// quick and dirty std::string tokenizer. does not support quotes, escape
// characters etc...

void tokenize_string(const std::string& str,
                     str_vec_t& tokens,
                     const std::string& delimiters = " ",
                     const bool keepEmpty = false);

/*
  tokenize_string2() - more featured (and less efficient) std::string tokenizer


  Arguments:

  str

     This is the original std::string that will be tokenized.

  result

     This std::vector holds all the generated tokens.

  delimiters

     The delimiters which will split the input std::string.

     Default: Space,TAB

  delimiters_preserve

     The delimiters which will split the input std::string.

     These delimiters will appear in the result as tokens.

     No default characters.

  quote

     The quote characters.

     The quote characters protect the enclosed text (matching quotes).

     Default: " and '

  esc

     The escape characters.

     These characters protect single characters.

     Default: backslash (\)

  empty_tokens

     If set to true, multiple adjacent delimeters will generate empty tokens

  keep_quotes

     If set to true, quote characters will be kept in the output std::strings

*/

void tokenize_string2(const std::string& str,
                      str_vec_t& result,
                      const std::string& delimiters = " \t",
                      const std::string& delimiters_preserve = "",
                      const std::string& quote = "\"'",
                      const std::string& esc = "\\",
                      bool empty_tokens = false,
                      bool keep_quotes = false);

void tokenize_string2(const std::string& str,
                      str_vec_t& result,
                      bool empty_tokens,
                      bool keep_quotes);

std::vector<std::string> split(const std::string& input,
                               const std::string& delim = ",",
                               bool keepEmpty = false);

} // namespace strutil
} // namespace toolbox
