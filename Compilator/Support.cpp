#include "Support.h"
#include <Windows.h>
std::map<unsigned short int, std::string> lexemType = {
    {1, "DIVIDER"},  {2, "IDENT"},  {3, "CONST"},  {4, "KEYWORD"}
};

std::vector<std::string> keyWords = {
    "main","bool","char", "do",
    "if", "int", "while", "return","iostream", "vector", "include", "stdlib", "proc"
};

std::vector<std::string> RegExpression = {
    "[A-za-z_]","[\\w_]","[\\d]","[\"]","[\[\\]\,\;\(\)\{\}\*\#\-]","[\\s]","[=><!/+]"
};

bool coincidenceRegExprWithCh(char c, std::string regExpr) {
    std::string input = std::string(1, c);
    std::regex regPattern(regExpr);
    return std::regex_match(input, regPattern);
}
bool isKeyWord(const std::string& obj) {

    return std::find(keyWords.begin(), keyWords.end(), obj) != keyWords.end();
}
/*
void printTree(const std::shared_ptr<Node>& node, int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    std::cout << "|- " << node->value << std::endl;

    for (size_t i = 0; i < node->children.size(); ++i) {
        if (i == node->children.size() - 1) {
            std::cout << std::string(2 * (level + 1), ' ') << "\\-";
        }
        else {
            std::cout << std::string(2 * (level + 1), ' ') << "|-";
        }
        printTree(node->children[i], level + 1);
    }
}*/


void printTree(const std::shared_ptr<Node>& node, int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    } 
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, level+1 | 0);
    //cout << "Hello world!" << endl;

    std::cout << "level " << level<<' ';
    std::cout<<node->value << std::endl;
    for (const auto& el : node->children)
    {
        printTree(el, level + 1);
    }
}