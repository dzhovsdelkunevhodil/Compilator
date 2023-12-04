#pragma once
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <regex>
//#include "lexAnalyzer.h"
extern std::map<unsigned short int, std::string> lexemType;

extern std::vector<std::string> keyWords;

extern std::vector<std::string> RegExpression;

enum State {
    BEGIN_STATE,
    ERROR,
    ID,
    CONST,
    DIVIDER,
    DSIGN
};

typedef struct  Lexema
{
    unsigned short int lexType;

    ///information about Lexema
    unsigned short int lexClass;
    unsigned short int n_col;
    unsigned short int n_row;
    unsigned short int numInTable;

    Lexema() : lexClass(0), lexType(0), n_row(0), n_col(0), numInTable(0) {}
    Lexema(unsigned short int lCl, unsigned short int lType, unsigned short int n_r, unsigned short int n_c, unsigned short int numIn)
        : lexClass(lCl), lexType(lType), n_row(n_r), n_col(n_c), numInTable(numIn) {}

    void display() {
        std::cout  << lexClass  << ' ' << lexType << ' ' << numInTable << "     ";
    }
    int getLexType() {
        return lexType;
    }

}Lexema;

class Node {
public:
    std::string value;
    std::vector<std::shared_ptr<Node>> children;

    Node(const std::string& value) : value(value) {}
    Node(const std::string& value, const std::vector<std::shared_ptr<Node>>& children) : value(value), children(children) {}
};


//void printTree(const std::shared_ptr<Node>& node, int level = 0);

void printTree(const std::shared_ptr<Node>& node, int level = 0);

bool coincidenceRegExprWithCh(char c, std::string pattern);

bool isKeyWord(const std::string& obj);