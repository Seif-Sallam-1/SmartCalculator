//
// Created by eseif on 06-Dec-25.
//

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

// 1. Define Token Types
enum TokenType {
    TOKEN_NUMBER,
    TOKEN_VARIABLE, // x, y, salary
    TOKEN_FUNCTION, // sin, cos, log
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_MUL,      // *
    TOKEN_DIV,      // /
    TOKEN_POWER,    // ^
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF       // End of File (Finished)
};

// 2. The Token Structure
struct Token {
    TokenType type;
    std::string text; // Stores the actual string ("55", "sin", "+")
};

// 3. The Lexer Class
class Lexer {
    std::string source;
    size_t pos = 0;

public:
    explicit Lexer(std::string src) : source(std::move(src)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < source.length()) {
            char current = source[pos];

            // A. Skip Whitespace
            if (std::isspace(current)) {
                pos++;
                continue;
            }

            // B. Handle Numbers (Digits)
            if (std::isdigit(current) || current == '.') {
                tokens.push_back(readNumber());
                continue;
            }

            // C. Handle Words (Variables or Functions)
            if (std::isalpha(current)) {
                tokens.push_back(readWord());
                continue;
            }

            // D. Handle Operators
            switch (current) {
                case '+': tokens.push_back({TOKEN_PLUS, "+"}); break;
                case '-': tokens.push_back({TOKEN_MINUS, "-"}); break;
                case '*': tokens.push_back({TOKEN_MUL, "*"}); break;
                case '/': tokens.push_back({TOKEN_DIV, "/"}); break;
                case '^': tokens.push_back({TOKEN_POWER, "^"}); break;
                case '(': tokens.push_back({TOKEN_LPAREN, "("}); break;
                case ')': tokens.push_back({TOKEN_RPAREN, ")"}); break;
                default:
                    std::cerr << "Unknown Character: " << current << std::endl;
            }
            pos++;
        }
        tokens.push_back({TOKEN_EOF, ""}); // Mark end of list
        return tokens;
    }

private:
    Token readNumber() {
        size_t start = pos;
        // Keep reading while it's a digit or a decimal point
        while (pos < source.length() && (std::isdigit(source[pos]) || source[pos] == '.')) {
            pos++;
        }
        return {TOKEN_NUMBER, source.substr(start, pos - start)};
    }

    Token readWord() {
        size_t start = pos;
        // Keep reading while it's a letter or digit (e.g., var1)
        while (pos < source.length() && std::isalnum(source[pos])) {
            pos++;
        }
        std::string word = source.substr(start, pos - start);

        // Check if it's a known function name
        if (word == "sin" || word == "cos" || word == "tan" || word == "log" || word == "ln" || word == "sqrt") {
            return {TOKEN_FUNCTION, word};
        }

        // Otherwise, it's a variable
        return {TOKEN_VARIABLE, word};
    }
};

#endif //LEXER_H