#include <cpp-toolbox/tokenize.h>

namespace toolbox {
namespace strutil {

// quick and dirty std::string tokenizer. does not support quotes, escape
// characters etc...

void
tokenize_string(const std::string& str,
                str_vec_t& tokens,
                const std::string& delimiters,
                const bool keepEmpty)
{
    if (str.empty())
        return;

    // Skip delimiters at beginning.
    std::string::size_type lastPos =
        keepEmpty ? 0 : str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    if (std::string::npos == pos) {
        // no delimeters, push whole std::string as a token
        tokens.push_back(str);
        return;
    }

    if (keepEmpty) {
        unsigned dsize = delimiters.size();
        do {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));

            // Skip delimiters.
            lastPos = pos + dsize;
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        } while (lastPos != std::string::npos + dsize);
    } else {
        while (std::string::npos != pos || std::string::npos != lastPos) {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));

            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
}

//    tokenize_string2() - based on zlib/libpng licensed (free) code by Jörg
//    Wiedenmann

void
tokenize_string2(const std::string& str,
                 std::vector<std::string>& result,
                 const std::string& delimiters,
                 const std::string& delimiters_preserve,
                 const std::string& quote,
                 const std::string& esc,
                 bool empty_tokens,
                 bool keep_quote_chars)
{
    if (str.empty())
        return;

    // clear the std::vector
    if (!result.empty())
        result.clear();

    std::string::size_type pos =
        0; // the current position (char) in the std::string

    char current_quote = 0; // the char of the current open quote
    bool quoted = false;    // indicator if there is an open quote
    std::string token;      // std::string buffer for the token

    bool token_complete = false; // indicates if the current token is
                                 // read to be added to the result std::vector

    std::string::size_type len = str.length(); // length of the input-string

    // for every char in the input-string

    while (len > pos) {
        // get the character of the std::string and reset the delimiter buffer

        char ch = str.at(pos); // buffer for the current character
        char delimiter = 0;    // the buffer for the delimiter char which
                               // will be added to the tokens if the delimiter
                               // is preserved

        delimiter = 0;

        // assume ch isn't a delimiter
        bool add_char = true;

        // check ...

        // ... if the delimiter is an escaped character
        bool escaped = false;     // indicates if the next char is protected
        if (false == esc.empty()) // check if esc-chars are  provided
        {
            if (std::string::npos != esc.find_first_of(ch)) {
                // get the escaped char
                ++pos;
                if (pos < len) // if there are more chars left
                {
                    // get the next one
                    ch = str.at(pos);

                    // add the escaped character to the token
                    add_char = true;
                } else // cannot get any more characters
                {
                    // don't add the esc-char
                    add_char = false;
                }

                // ignore the remaining delimiter checks
                escaped = true;
            }
        }

        // ... if the delimiter is a quote
        if (false == quote.empty() && false == escaped) {
            // if quote chars are provided and the char isn't protected
            if (std::string::npos != quote.find_first_of(ch)) {
                // if not quoted, set state to open quote and set
                // the quote character
                if (false == quoted) {
                    quoted = true;
                    current_quote = ch;

                    // don't add the quote-char to the token
                    add_char = keep_quote_chars;
                } else // if quote is open already
                {
                    // check if it is the matching character to close it
                    if (current_quote == ch) {
                        // close quote and reset the quote character
                        quoted = false;
                        current_quote = 0;

                        // don't add the quote-char to the token
                        add_char = keep_quote_chars;
                    }
                } // else
            }
        }

        // ... if the delimiter isn't preserved
        if (false == delimiters.empty() && false == escaped &&
            false == quoted) {
            // if a delimiter is provided and the char isn't protected by
            // quote or escape char
            if (std::string::npos != delimiters.find_first_of(ch)) {
                // if ch is a delimiter and the token std::string isn't empty
                // the token is complete
                //                if ( false == token.empty() ) // BUGFIX:
                //                2006-03-04
                {
                    token_complete = true;
                }

                // don't add the delimiter to the token
                add_char = false;
            }
        }

        // ... if the delimiter is preserved - add it as a token
        bool add_delimiter = false;
        if (false == delimiters_preserve.empty() && false == escaped &&
            false == quoted) {
            // if a delimiter which will be preserved is provided and the
            // char isn't protected by quote or escape char
            if (std::string::npos != delimiters_preserve.find_first_of(ch)) {
                // if ch is a delimiter and the token std::string isn't empty
                // the token is complete
                if (false == token.empty()) // BUGFIX: 2006-03-04
                {
                    token_complete = true;
                }

                // don't add the delimiter to the token
                add_char = false;

                // add the delimiter
                delimiter = ch;
                add_delimiter = true;
            }
        }

        // add the character to the token
        if (true == add_char) {
            // add the current char
            token.push_back(ch);
        }

        // add the token if it is complete
        //        if ( true == token_complete && false == token.empty() )
        if (true == token_complete) {
            // add the token std::string
            result.push_back(token);

            // clear the contents
            token.clear();

            // build the next token
            token_complete = false;
        }

        // add the delimiter
        if (true == add_delimiter) {
            // the next token is the delimiter
            std::string delim_token;
            delim_token.push_back(delimiter);
            result.push_back(delim_token);

            // REMOVED: 2006-03-04, Bugfix
        }

        // repeat for the next character
        ++pos;
    } // while

    // add the final token
    if (false == token.empty()) {
        result.push_back(token);
    }
}

void
tokenize_string2(const std::string& str,
                 str_vec_t& result,
                 bool empty_tokens,
                 bool keep_quotes)
{
    tokenize_string2(
        str, result, " \t", "", "\"'", "\\", empty_tokens, keep_quotes);
}

std::vector<std::string>
split(const std::string& input, const std::string& delim, bool keepEmpty)
{
    std::vector<std::string> results;
    tokenize_string(input, results, delim, keepEmpty);
    return results;
}

}
}
