#include "syntaxAnalyzer.h"
#include "Support.h"

/*
S = Подкл. библиотек Ф-и Тип "main" "(" ")" "{" Тело "}".

Подк. Библиотек = "#" "include"  "<" "stdlib" ">".

Ф-и = Тип "proc" Идентификатор "(" Арг. ф-и ")" "{"  Тело  "}" ";" .

Арг. ф-и = Тип Идентификатор ["," Арг. ф-и].

Тело = { Оператор }.

Оператор = Присваивание | Цикл | Инкремент | Объявление переменных | Возврат 
зн-й.

Возврат зн-й = "return" Идентификатор";".

Объявление переменных = Тип  Идентификатор ";".

Присваивание = Идентификатор "=" Выражение ";".

Выражение = Идентификатор | Вызов ф-и | Число Знак { Выражение }.

Знак = "+" | "*" | "-"

Инкремент = Идентификатор "++" ";". 

Цикл = "do" "{"  Тело  "}" "while" "(" Условие ")" ";".

Вызов ф-и = Идентификатор "(" Идентификатор { "," Идентификатор } ")".

Тип = "int" | "bool" | "char".

Идентификатор = Буква [Буква | Цифра | "_"].

Буква = "A" | "B" | "C" | … | "Z" | "a" | "b" | "c" | ... | "z ".

Число = Цифра | Цифра Число.

Цифра = " 0 " | "1" | … | "9".

}*/

void syntaxAnalyzer::analysis()
{
    ParseTree = analysisS();
}

std::string syntaxAnalyzer::CurrentLexClass()
{
    return lexemClass[lexStream[lexIndex].lexClass];
}

std::shared_ptr<Node> syntaxAnalyzer::analysisS()
{
    if (IsMatch("#")) {
        std::vector<std::shared_ptr<Node>> children;
        while (IsMatch("#")) {
            children.push_back(analysisLibrary());
        }

        children.push_back(analysisFunction());
        children.push_back(Match("int"));
        children.push_back(Match("main"));
        children.push_back(Match("("));
        children.push_back(Match(")"));
        children.push_back(Match("{"));
        children.push_back(analysisBody());
        children.push_back(Match("}"));

        return std::make_shared<Node>("---Programm---", children);
    }
    else {
        try {
            std::cout << "Unexpected lexema: " + CurrentLexClass() << '\n';
            throw std::runtime_error("Unexpected lexema: " + CurrentLexClass());
        }
        catch (...) {
            error();
            exit(-1);
        }
    }
}

std::shared_ptr<Node> syntaxAnalyzer::analysisBody()
{
    std::vector<std::shared_ptr<Node>> children;
    while (IsMatch({ "int", "char", "bool", "return", "do"}) || IsMatch(reprIdTable)) {
        children.push_back(analysisOperator());
        //countOperators++;
    }
    return std::make_shared<Node>("-----Body-----", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisLibrary()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch("#")) {
        children.push_back(Match("#"));
        children.push_back(Match("include"));
        children.push_back(Match("<"));
        children.push_back(Match({ "stdlib", "iostream" , "vector" }));
        children.push_back(Match(">"));
    }
    return std::make_shared<Node>("----Library---", children);
}


std::shared_ptr<Node> syntaxAnalyzer::analysisOperator()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch({ "int", "char", "bool" })) {
        children.push_back(analysisDeclaration());
    }
    else if (IsMatch(reprIdTable)) {
        if (lexemClass[lexStream[lexIndex + 1].lexClass] == "=") {
            try {
                for (int i = 0; i < reprIdTable.size();i++) {
                    if (CurrentLexClass() == reprIdTable[i]) {
                        //inn[i]++;
                        if (inn[i] < 1) {
                            std::cout << CurrentLexClass() << " :---: ";
                            std::cout << "Don't declaration\n";
                            throw std::runtime_error("Don't declaration");
                            break;
                        }
                        break;
                    }
                }
            }
            catch (...) {
                error();
                exit(-1);
            }
            children.push_back(analysisAssignment());
        }
        else if (lexemClass[lexStream[lexIndex + 1].lexClass] == "++") {
            try {
                for (int i = 0; i < reprIdTable.size();i++) {
                    if (CurrentLexClass() == reprIdTable[i]) {
                        //inn[i]++;
                        if (inn[i] < 1) {
                            std::cout << CurrentLexClass() << " :---: ";
                            std::cout << "Don't declaration\n";
                            throw std::runtime_error("Don't declaration");
                            break;
                        }
                        break;
                    }
                }
            }
            catch (...) {
                error();
                exit(-1);
            }
            children.push_back(analysisInc());
        }
        else {
            try {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "Cringe Error\n";
                throw std::runtime_error("Cringe Error");
            }
            catch (...) {
                error();
                exit(-1);
            }
        }
    }
    else if (IsMatch("do")) {
        children.push_back(analysisLoop());
    }
    else if (IsMatch("return")) {
        children.push_back(analysisReturn());
    }
    return std::make_shared<Node>("---Operator---", children);
}
std::shared_ptr<Node> syntaxAnalyzer::analysisInc()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(analysisIdentif());
    children.push_back(Match("++"));
    children.push_back(Match(";"));
    return std::make_shared<Node>("---Increase---", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisDeclaration()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch("char")) {
        children.push_back(Match("char"));
        try {
            for (int i = 0; i < reprIdTable.size();i++) {
                if (CurrentLexClass() == reprIdTable[i]) {
                    IdTable.push_back(reprIdTable[i]);
                    inn[i]++;
                    if (inn[i] > 1) {
                        std::cout << CurrentLexClass() << " :---: ";
                        std::cout << "Redeclaration\n";
                        throw std::runtime_error("Redeclaration");
                        break;
                    }
                }
            }
        }
        catch (...) {
            error();
            exit(-1);
        }

        idType.push_back(children[0]->value);
        if (IsMatch(reprIdTable)) {
            children.push_back(analysisIdentif());
        }
        else {
            try {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "It's not ID\n";
                throw std::runtime_error("It's not ID");
            }
            catch (...) {
                error();
                exit(-1);
            }
        }
        children.push_back(Match("["));
        children.push_back(analysisConst());
        children.push_back(Match("]"));
    }
    else {
        children.push_back(Match({ "int", "bool", "hzzz"}));
        try {
            for (int i = 0; i < reprIdTable.size();i++) {
                if (CurrentLexClass() == reprIdTable[i]) {
                    IdTable.push_back(reprIdTable[i]);
                    inn[i]++;
                    if (inn[i] > 1) {
                        std::cout << CurrentLexClass() << " :---: ";
                        std::cout << "Redeclaration\n";
                        throw std::runtime_error("Redeclaration");
                        break;
                    }
                    break;
                }
            }
        }
        catch (...) {
            error();
            exit(-1);
        }
        idType.push_back(children[0]->value);
        if (IsMatch(reprIdTable)) {
            children.push_back(analysisIdentif());
        }
        else {
            try {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "It's not ID\n";
                throw std::runtime_error("It's not ID");
            }
            catch (...) {
                error();
                exit(-1);
            }
        }
    }
    children.push_back(Match(";"));
    return std::make_shared<Node>("--Declaration-", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisAssignment()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(analysisIdentif());
    children.push_back(Match("="));
    children.push_back(analysisExpression());
    children.push_back(Match(";"));
    return std::make_shared<Node>("--Assignment--", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisExpression()
{
    std::vector<std::shared_ptr<Node>> children;
    //countOperators++;
    if (IsMatch(reprIdTable)) {
        if (lexemClass[lexStream[lexIndex + 1].lexClass] == "(") {
            for (int i = 0; i < reprIdTable.size();i++) {
                if (CurrentLexClass() == reprIdTable[i] && inn[i] == 1) {
                    children.push_back(analysisCallF());
                    if (IsMatch({ "+" , "-", "*" })) {
                        children.push_back(Match({ "+" , "-", "*" }));
                        countOperators++;
                        children.push_back(analysisExpression());
                    }
                    break;
                }
                else {
                    try {
                        std::cout << CurrentLexClass() << " :---: ";
                        std::cout << "This ID is not declaration \n";
                        throw std::runtime_error("This ID is not declaration");
                        break;
                    }
                    catch (...) {
                        error();
                        exit(-1);
                       
                    }
                }
            }
        }
        else if (lexemClass[lexStream[lexIndex + 1].lexClass] == "+" || lexemClass[lexStream[lexIndex + 1].lexClass] == "-"
            || lexemClass[lexStream[lexIndex + 1].lexClass] == "*") {
            for (int i = 0; i < reprIdTable.size();i++) {
               // std::cout << CurrentLexClass() << inn[i] << reprIdTable[i];
                if (CurrentLexClass() == reprIdTable[i]) {
                    if(inn[i] != 1){
                    try {
                        std::cout << CurrentLexClass() << " :---: ";
                        std::cout << "This ID is not declaration \n";
                        throw std::runtime_error("This ID is not declaration");
                        break;
                    }
                    catch (...) {
                        error();
                        exit(-1);
                    }
                    }
                    children.push_back(analysisIdentif());
                    if (IsMatch({ "+" , "-", "*" })) {
                        countOperators++;
                        children.push_back(Match({ "+" , "-", "*" }));
                        children.push_back(analysisExpression());
                    }
                   // children.push_back(Match({ "+" , "-", "*" }));
                    //children.push_back(analysisExpression());
                }
            }
        }
        else {
            for (int i = 0; i < reprIdTable.size();i++) {
                // std::cout << CurrentLexClass() << inn[i] << reprIdTable[i];
                if (CurrentLexClass() == reprIdTable[i]) {
                    if (inn[i] != 1) {
                        try {
                            std::cout << CurrentLexClass() << " :---: ";
                            std::cout << "This ID is not declaration \n";
                            throw std::runtime_error("This ID is not declaration");
                            break;
                        }
                        catch (...) {
                            error();
                            exit(-1);
                        }
                    }
                    children.push_back(analysisIdentif());
                    
                    // children.push_back(Match({ "+" , "-", "*" }));
                     //children.push_back(analysisExpression());
                }
            }


          //  children.push_back(analysisIdentif());
        }
    }
    else if (IsMatch(std::vector<std::string>{"CONST"})) {
        children.push_back(analysisConst());
    }
    return std::make_shared<Node>("--Expression--", children);
}


std::shared_ptr<Node> syntaxAnalyzer::analysisCallF()
{
    int arg1 = 0;
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(analysisIdFunc());
    children.push_back(Match("("));
    while (IsMatch(reprIdTable)) {
        children.push_back(analysisIdentif());
        arg1++;
        if (!IsMatch(")"))
            children.push_back(Match(","));
        if (arg1 != countArg && IsMatch(")")){
            try {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "Unmatch count of arguments in function/callFunction\n";
                throw std::runtime_error("Unmatch count of arguments in function/callFunction");
            }
            catch (...) {
                error();
                exit(-1);
            }

        }
    }
    if (arg1 != countArg && IsMatch(")")) {
        try {
            std::cout << CurrentLexClass() << " :---: ";
            std::cout << "Unmatch count of arguments in function/callFunction\n";
            throw std::runtime_error("Unmatch count of arguments in function/callFunction");
        }
        catch (...) {
            error();
            exit(-1);
        }
    }
    children.push_back(Match(")"));
    return std::make_shared<Node>("-CallFunction-", children);
}


std::shared_ptr<Node> syntaxAnalyzer::analysisConst()
{
    std::vector<std::shared_ptr<Node>> children;
    //children.push_back(Match("CONST"));
    if (IsMatch("CONST")) {
        children.push_back(std::make_shared<Node>(constTable[lexStream[lexIndex].numInTable]));
        NextLexema();
    }
    else {
        children.push_back(Match("CONST"));
    }
    return std::make_shared<Node>("-----CONST----", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisIdentif()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(std::make_shared<Node>(reprIdTable[lexStream[lexIndex].numInTable]));
    NextLexema();
    return std::make_shared<Node>("-------ID-------", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisCondition()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch(reprIdTable)) {
        if (lexemClass[lexStream[lexIndex + 1].lexClass] == "==" || lexemClass[lexStream[lexIndex + 1].lexClass] == "!=" || 
            lexemClass[lexStream[lexIndex + 1].lexClass] == ">" || lexemClass[lexStream[lexIndex + 1].lexClass] == "<" || 
            lexemClass[lexStream[lexIndex + 1].lexClass] == ">=" || lexemClass[lexStream[lexIndex + 1].lexClass] == "<=") {
            children.push_back(analysisExpression());
            children.push_back(Match({ "==", "!=", ">", "<", ">=", "<=" }));
            children.push_back(analysisExpression());
        }
        else {
            children.push_back(analysisIdentif());
        }
    }
    else {
        try {
            std::cout << "Condition cannot be empty\n";
           // error();
            throw std::runtime_error("Condition cannot be empty");
        }
        catch (...) {
            error();
            exit(-1);
        }
    }
    return std::make_shared<Node>("---Condition--", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisLoop()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match("do"));
    children.push_back(Match("{"));
    children.push_back(analysisBody());
    children.push_back(Match("}"));
    children.push_back(Match("while"));
    children.push_back(Match("("));
    children.push_back(analysisCondition());
    children.push_back(Match(")"));
    children.push_back(Match(";"));
    return std::make_shared<Node>("-----Loop-----", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisReturn()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match("return"));
    children.push_back(analysisIdentif());
    children.push_back(Match(";"));
    return std::make_shared<Node>("----Return----", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisFunction()
{
    countArg = 0;
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match({ "int", "char", "bool" }));
    idTypeFunc.push_back(children[0]->value);
    children.push_back(Match("proc"));
    ////////////ffffuuuuunnnncccc/////////////

    for (int i = 0; i < reprIdTable.size();i++) {
        if (CurrentLexClass() == reprIdTable[i]) {
            IdFuncTable.push_back(reprIdTable[i]);
            //std::cout << reprIdTable[i];
            //reprIdTable[i].erase();
            //std::cout << reprIdTable[i];
            //innF.push_back(1);
            inn[i]++;
            if (inn[i] > 1) {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "Redeclaration\n";
                throw std::runtime_error("Redeclaration");
                break;
            }
            //inn[i].erase();
            break;
        }
    }

    children.push_back(analysisIdFunc());

   // children.push_back(analysisIdentif());
    //inn;
    children.push_back(Match("("));
    while (IsMatch({ "int", "char", "bool" })) {
        children.push_back(analysisArguments());
        countArg++;
    }
    children.push_back(Match(")"));
    children.push_back(Match("{"));
    children.push_back(analysisBody());
    children.push_back(Match("}"));
    children.push_back(Match(";"));
    return std::make_shared<Node>("---Function---", children);
}

std::shared_ptr<Node> syntaxAnalyzer::analysisIdFunc() {
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(std::make_shared<Node>(reprIdTable[lexStream[lexIndex].numInTable]));
    NextLexema();
    return std::make_shared<Node>("---IdFunc--", children);
}


std::shared_ptr<Node> syntaxAnalyzer::analysisArguments()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match({ "int", "char", "bool" }));
    idType.push_back(children[0]->value);
    for (int i = 0; i < reprIdTable.size();i++) {
        if (CurrentLexClass() == reprIdTable[i]) {
            IdTable.push_back(reprIdTable[i]);
            //std::cout << reprIdTable[i];
            //reprIdTable[i].erase();
            //std::cout << reprIdTable[i];
            //innF.push_back(1);
            inn[i]++;
            if (inn[i] > 1) {
                std::cout << CurrentLexClass() << " :---: ";
                std::cout << "Redeclaration\n";
                throw std::runtime_error("Redeclaration");
                break;
            }
            //inn[i].erase();
            break;
        }
    }
    children.push_back(analysisIdentif());
    if (!IsMatch(")")) {
        children.push_back(Match(","));
    }
    return std::make_shared<Node>("---Arguments--", children);
}

std::shared_ptr<Node> syntaxAnalyzer::Match(const std::string& expectedLex)
{
    if (lexIndex < lexStream.size() && CurrentLexClass() == expectedLex) {
        std::shared_ptr<Node> node = std::make_shared<Node>(CurrentLexClass());
        NextLexema();
        return node;
    }
    else {
        try {
            std::cout << "Expected lexema: " + expectedLex + "  Current lexema: " + CurrentLexClass() << '\n';
            throw std::runtime_error("Unexpected lexema: " + CurrentLexClass());
        }
        catch (...) {
            error();
            exit(-1);
        }
    }
}

std::shared_ptr<Node> syntaxAnalyzer::Match(const std::vector<std::string>& expectedLexemes)
{
    try {
        if (lexIndex >= lexStream.size()) {
            std::cout << "No more lexema available.\n";
            throw std::runtime_error("No more lexema available.");
        }
    }
    catch (...) {
        error();
        exit(-1);
    }

    for (int i = 0; i < expectedLexemes.size(); i++) {
        if (CurrentLexClass() == expectedLexemes[i]) {
            std::shared_ptr<Node> node = std::make_shared<Node>(CurrentLexClass());
            NextLexema();
            return node;
        }
    }

    return nullptr;
}

bool syntaxAnalyzer::IsMatch(const std::vector<std::string>& expectedLexemes)
{
    for (int i = 0; i < expectedLexemes.size(); i++) {
        if (CurrentLexClass() == expectedLexemes[i]) {
            return true;
        }
    }
    return false;
}

bool syntaxAnalyzer::IsMatch(const std::string& expectedLex)
{
    return CurrentLexClass() == expectedLex;
}

void syntaxAnalyzer::NextLexema()
{
    try {
        if (lexIndex < lexStream.size()) {
            lexIndex++;
        }
        else if (lexemClass[lexStream[lexIndex - 1].lexClass] != "}") {
            std::cout << "No more lexema available.\n";
            throw std::runtime_error("No more lexema available.");
        }
    }
    catch (...) {
        error();
        exit(-1);
    }
}

void syntaxAnalyzer::error() {
    int i = 0;
    std::cout << "Error in row #" << lexStream[lexIndex].n_row;
}


void syntaxAnalyzer::PrintTable()
{
    std::cout << '\n' << '#' << "\t" << "Name of Identifier" << "\t" << "Type" << std::endl;
    for (int i = 0; i < IdTable.size() && i < idType.size(); i++) {
        std::cout << i << "\t" << std::setw(20) << std::left << IdTable[i] << "\t" << idType[i] << std::endl;
    }

    std::cout << '\n' << '\n' << '#' << "\t" << "Name of IdentiFunc" << "\t" << "Type" << std::endl;
    for (int i = 0; i < IdFuncTable.size() && i < idTypeFunc.size(); i++) {
        std::cout << i << "\t" << std::setw(20) << std::left << IdFuncTable[i] << "\t" << idTypeFunc[i] << std::endl;
    }
}
