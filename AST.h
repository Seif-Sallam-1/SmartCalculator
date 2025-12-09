//
// Created by eseif on 06-Dec-25.
//

#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include <stdexcept>

using SymbolTable = std::map<std::string, long double>;

// 1. Abstract Base Class
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual long double evaluate(SymbolTable& vars) = 0;

    // --- NEW FOR GUI ---
    // Returns the text to display in the circle (e.g., "+", "5", "sin")
    virtual std::string getLabel() = 0;

    // Returns list of children so the GUI knows where to draw lines
    virtual std::vector<ASTNode*> getChildren() = 0;
};

// 2. Leaf Node: Number
class NumberNode : public ASTNode {
    long double value;
public:
    explicit NumberNode(long double val) : value(val) {}
    long double evaluate(SymbolTable& vars) override { return value; }

    std::string getLabel() override {
        // Remove trailing zeros for cleaner display
        std::string s = std::to_string(value);
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (s.back() == '.') s.pop_back();
        return s;
    }
    std::vector<ASTNode*> getChildren() override { return {}; } // No children
};

// 3. Leaf Node: Variable
class VariableNode : public ASTNode {
    std::string name;
public:
    explicit VariableNode(std::string varName) : name(std::move(varName)) {}
    long double evaluate(SymbolTable& vars) override {
        if (vars.find(name) == vars.end()) throw std::runtime_error("Unknown var: " + name);
        return vars[name];
    }
    std::string getLabel() override { return name; }
    std::vector<ASTNode*> getChildren() override { return {}; }
};

// 4. Internal Node: Binary Operation
class BinaryOpNode : public ASTNode {
    ASTNode* left;
    ASTNode* right;
    char op;
public:
    BinaryOpNode(ASTNode* l, char o, ASTNode* r) : left(l), op(o), right(r) {}
    ~BinaryOpNode() override { delete left; delete right; }

    long double evaluate(SymbolTable& vars) override {
        long double l = left->evaluate(vars);
        long double r = right->evaluate(vars);
        switch (op) {
            case '+': return l + r;
            case '-': return l - r;
            case '*': return l * r;
            case '/': if (r == 0) throw std::runtime_error("Div by zero"); return l / r;
            case '^': return std::pow(l, r);
            default: return 0;
        }
    }
    std::string getLabel() override { return std::string(1, op); } // Return "+"
    std::vector<ASTNode*> getChildren() override { return {left, right}; }
};

// 5. Unary Node (Functions)
class UnaryOpNode : public ASTNode {
    ASTNode* child;
    std::string funcName;
public:
    UnaryOpNode(std::string name, ASTNode* c) : funcName(std::move(name)), child(c) {}
    ~UnaryOpNode() override { delete child; }

    long double evaluate(SymbolTable& vars) override {
        long double val = child->evaluate(vars);
        if (funcName == "sin") return std::sin(val);
        if (funcName == "cos") return std::cos(val);
        if (funcName == "tan") return std::tan(val);
        if (funcName == "log") return std::log10(val);
        if (funcName == "sqrt") return std::sqrt(val);
        return 0;
    }
    std::string getLabel() override { return funcName; }
    std::vector<ASTNode*> getChildren() override { return {child}; }
};


#endif //SMARTCALCULATOR_AST_H