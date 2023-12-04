#pragma once
#include <fstream>
#include "Support.h"

//неправильное исполльзование функции или отсутствие аргументов
///переопределение////////////////////
///нет типа для переменной////////////
///ключевое слово как переменная//////
/// буквы вместо цифры в размере//////
/// int вместо char

#include <iomanip>

class syntaxAnalyzer
{
private:
	
	std::vector<Lexema> lexStream;
	std::vector<std::string> reprIdTable;
	std::vector<std::string> IdFuncTable;
	std::vector<std::string> IdTable;
	std::vector<std::string> constTable;
	std::vector<std::string> lexemClass;
	std::vector<int> inn;
	int countOperators;
	std::vector<int> innF;

	int countArg;
	
	size_t lexIndex;
	std::shared_ptr<Node> ParseTree;
	std::vector<std::string> idType;
	std::vector<std::string> idTypeFunc;

public:
	void displayStreamLexem() {
		for (int i = 0; i < lexStream.size();i++) {
			lexStream[i].display();
			if (i % 5 == 4) {
				std::cout << std::endl << std::endl;
			}
		}
		std::cout << std::endl << std::endl;
	}

	syntaxAnalyzer(std::vector<Lexema> lexStream) : lexStream(lexStream), lexIndex(0), 
		ParseTree(nullptr), countArg(0), countOperators(0) {}

	syntaxAnalyzer(std::vector<Lexema> lexStream, std::vector<std::string> reprIdTable,
		std::vector<std::string> constTable, std::vector<std::string> lexemClass, std::vector<int> inn)
		: lexStream(lexStream), lexIndex(0), ParseTree(nullptr), reprIdTable(reprIdTable),
		constTable(constTable), lexemClass(lexemClass), inn(inn), countArg(0), countOperators(0) {}

	void analysis(); 
	std::string CurrentLexClass(); 
	std::shared_ptr<Node> GetParseTree() { return ParseTree; }
	std::vector<std::string> GetidType() { return std::move(idType); }
	std::vector<std::string> GetidTypeFunc() { return std::move(idTypeFunc); }
	std::vector<std::string> GetidTable() { return std::move(IdTable); }
	std::vector<std::string> GetidFuncTable() { return std::move(IdFuncTable); }
	int GetCountOperators() { return countOperators; }


	std::shared_ptr<Node> analysisS();
	std::shared_ptr<Node> analysisBody();
	std::shared_ptr<Node> analysisOperator();
	std::shared_ptr<Node> analysisDeclaration();
	std::shared_ptr<Node> analysisAssignment();
	std::shared_ptr<Node> analysisExpression();
	std::shared_ptr<Node> analysisCallF();
	std::shared_ptr<Node> analysisConst();
	std::shared_ptr<Node> analysisIdFunc();
	std::shared_ptr<Node> analysisIdentif();
	std::shared_ptr<Node> analysisCondition();
	std::shared_ptr<Node> analysisLoop();
	std::shared_ptr<Node> analysisReturn();
	std::shared_ptr<Node> analysisLibrary();
	std::shared_ptr<Node> analysisInc();
	std::shared_ptr<Node> analysisFunction();
	std::shared_ptr<Node> analysisArguments();
	void error();
	std::shared_ptr<Node> Match(const std::string& expectedLex);
	std::shared_ptr<Node> Match(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::string& expectedLex);
	void NextLexema();
	void PrintTable();
};