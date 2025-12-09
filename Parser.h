//
// Created by eseif on 06-Dec-25.
//

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <stdexcept>
#include "AST.h"
#include "Lexer.h"

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

public:
    explicit Parser(std::vector<Token> t) : tokens(std::move(t)) {}

    // The main entry point to start parsing
    ASTNode* parse() {
        return parseExpression();
    }

private:
    // Helper: Get current token
    Token current() {
        if (pos >= tokens.size()) return {TOKEN_EOF, ""};
        return tokens[pos];
    }

    // Helper: Move to next token
    void advance() {
        pos++;
    }

    // Helper: Check if current token matches a type, then consume it
    bool match(TokenType type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }

    // Helper: Require a specific token or throw error (e.g., closing parenthesis)
    void consume(TokenType type, const std::string& message) {
        if (current().type == type) {
            advance();
        } else {
            throw std::runtime_error("Parse Error: " + message);
        }
    }

    // --- LEVEL 1: Expressions (+ and -) ---
    ASTNode* parseExpression() {
        ASTNode* left = parseTerm();

        while (current().type == TOKEN_PLUS || current().type == TOKEN_MINUS) {
            char op = current().text[0]; // '+' or '-'
            advance();
            ASTNode* right = parseTerm();
            left = new BinaryOpNode(left, op, right);
        }
        return left;
    }

    // --- LEVEL 2: Terms (* and /) ---
    ASTNode* parseTerm() {
        ASTNode* left = parsePower();

        while (current().type == TOKEN_MUL || current().type == TOKEN_DIV) {
            char op = current().text[0]; // '*' or '/'
            advance();
            ASTNode* right = parsePower();
            left = new BinaryOpNode(left, op, right);
        }
        return left;
    }

    // --- LEVEL 3: Power (^) ---
    // Note: Power is usually right-associative (2^3^4 = 2^(3^4)),
    // but for simplicity here we treat it like * or /.
    ASTNode* parsePower() {
        ASTNode* left = parseFactor();

        while (current().type == TOKEN_POWER) {
            char op = '^';
            advance();
            ASTNode* right = parseFactor();
            left = new BinaryOpNode(left, op, right);
        }
        return left;
    }

    // --- LEVEL 4: Factors (Numbers, Variables, Functions, Parentheses) ---
    ASTNode* parseFactor() {
        Token t = current();

        // 1. Numbers (55, 2.5)
        if (t.type == TOKEN_NUMBER) {
            advance();
            return new NumberNode(std::stold(t.text));
        }

        // 2. Variables (x, salary)
        if (t.type == TOKEN_VARIABLE) {
            advance();
            return new VariableNode(t.text);
        }

        // 3. Scientific Functions (sin, cos)
        if (t.type == TOKEN_FUNCTION) {
            std::string funcName = t.text;
            advance(); // consume "sin"
            consume(TOKEN_LPAREN, "Expected '(' after function name");
            ASTNode* child = parseExpression(); // Recursively parse inside
            consume(TOKEN_RPAREN, "Expected ')' after function argument");
            return new UnaryOpNode(funcName, child);
        }

        // 4. Parentheses ( )
        if (t.type == TOKEN_LPAREN) {
            advance(); // eat '('
            ASTNode* node = parseExpression(); // Logic inside can be complex
            consume(TOKEN_RPAREN, "Expected ')'"); // eat ')'
            return node;
        }

        throw std::runtime_error("Unexpected token: " + t.text);
    }
};

#endif //PARSER_H