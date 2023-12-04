#pragma once
#include <fstream>
#include "Support.h"

class lexAnalyzer
{
    std::ifstream inFile;
    std::vector <Lexema> lexStream;
    unsigned short int numberInRow;
    unsigned short int numberInColumn;
    std::vector<std::string> reprIdTable;
    std::vector<std::string> constTable;
    std::vector<std::string> lexemClass;
    std::vector<int> inn;

public:
    lexAnalyzer(const std::string& filename);
    lexAnalyzer();
    ~lexAnalyzer();

    std::vector<Lexema>& getLexStream() {
        return lexStream;
    }

    std::vector<std::string>& getLexemClass() {
        return lexemClass;
    }

    bool analysis();
    void displayTables();
    unsigned short int checkReprId(const std::string& buff); // проверка и добавление в таблицу представлений Id
    unsigned short int checkLexemClass(const std::string& buff); // проверка таблица представления лексем
    unsigned short int checkConst(const std::string& buff);// проверка таблицы констант
    std::vector<Lexema> GetLexStream() { return std::move(lexStream); }
    std::vector<std::string> GetReprIdTable() { return std::move(reprIdTable); }
    std::vector<std::string> GetConstTable() { return std::move(constTable); }
    std::vector<std::string> GetLexemClasses() { return std::move(lexemClass); }
    std::vector<int> GetInn() { return std::move(inn); }
};
