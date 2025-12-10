#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

using namespace std;

class AST {
public:
    class BNode {
    public:
        string data;
        BNode* left;
        BNode* right;
        BNode(string val, BNode* l = nullptr, BNode* r = nullptr);
        ~BNode();
    };

private:
    BNode* head;
    vector<string> postfixContainer;
    int getPrecedence(const string& op);
    bool isOperator(const string& part);
    double calculateRecursive(BNode* node);

public:
    AST(string expression);
    ~AST();
    void buildTree();
    double calculate();
    BNode* getRoot();
};

#endif