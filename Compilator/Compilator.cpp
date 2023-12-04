#include "lexAnalyzer.h"
#include "syntaxAnalyzer.h"
#include "asmAnalyzer.h"


#include <Windows.h>
using namespace std;
#define Syntax
#define Print


int main() {
    bool f = false;
    std::cout << "lex Analysis\n";
    lexAnalyzer* lex = new lexAnalyzer;
    if (lex->analysis()) {
        f = true;
    }
#ifdef Print
    lex->displayTables();
#endif // Print

#ifdef Syntax
    
    if (f) {
        std::cout << "\n\n\n\nsyntax Analysis\n";
        syntaxAnalyzer* syntax = new syntaxAnalyzer(lex->GetLexStream(), lex->GetReprIdTable(), lex->GetConstTable(), 
            lex->GetLexemClasses(), lex->GetInn());

#ifdef Print
        syntax->displayStreamLexem();
#endif // Print
        
        syntax->analysis();
        printTree(syntax->GetParseTree());

        HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsoleHandle, 15 | 0);

#ifdef Print
        syntax->PrintTable();
#endif // Print

        asmAnalyzer* asmAn = new asmAnalyzer(syntax->GetParseTree(), syntax->GetidType(), syntax->GetidTypeFunc(),
            syntax->GetidTable(), syntax->GetidFuncTable(), syntax->GetCountOperators());

        asmAn->analysis();

        delete syntax;
        delete asmAn;
    }
#endif // Syntax

    delete lex;

    return 0;
}
