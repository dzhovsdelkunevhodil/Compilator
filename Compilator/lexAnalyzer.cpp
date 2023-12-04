#include "lexAnalyzer.h"

lexAnalyzer::lexAnalyzer() {

    inFile.open("input.txt", std::ios::in);

    if (!inFile.is_open()) {
        std::cerr << "File don't open!" << std::endl;
    }
    numberInRow = 1;
    numberInColumn = 1;
}

lexAnalyzer::lexAnalyzer(const std::string& filename) {

    inFile.open(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cerr << "File don't open!" << std::endl;
    }
    numberInRow = 1;
    numberInColumn = 1;
}

lexAnalyzer::~lexAnalyzer() {
    inFile.close();
}

bool lexAnalyzer::analysis() {
    State curState = BEGIN_STATE;
    char c;
    inFile.get(c);
    bool Flag = true;
    while (!inFile.eof() && Flag != false) {
        switch (curState)
        {
        case BEGIN_STATE: {
            while ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\0'))
            {
                if (c == '\n') {
                    numberInColumn++;
                    numberInRow = 0;
                }
                inFile.get(c);
                numberInRow++;

                if (inFile.eof()) {
                    break;
                }
            }

            if (inFile.eof()) {
                Flag = true;
                break;
            }

            if (coincidenceRegExprWithCh(c, RegExpression[4])) { //DIVIDER 
                curState = DIVIDER;
            }
            else if (coincidenceRegExprWithCh(c, RegExpression[0])) { // ID, KEYWORD
                curState = ID;
            }
            else if (coincidenceRegExprWithCh(c, RegExpression[2])) { // CONST
                curState = CONST;
            }
            else if (coincidenceRegExprWithCh(c, RegExpression[6])) { // DSIGN
                curState = DSIGN;
            }
            else {
                std::cout << "This not founded: " << c;
                std::cin >> c;
            }

            break;
        }

        case ID: {
            std::string buff;
            Lexema lex;
            unsigned short int saveNInRow = numberInRow;

            while (coincidenceRegExprWithCh(c, RegExpression[1])) {
                buff += c;
                inFile.get(c);
                numberInRow++;

                if (inFile.eof()) {
                    break;
                }
            }

            if (isKeyWord(buff)) {
                lex = { checkLexemClass(buff), 4, numberInColumn, saveNInRow, 0 }; // kword, colNum, rowNum, inTableNum
            }
            else {
                lex = { checkLexemClass(buff), 2, numberInColumn, saveNInRow, checkReprId(buff) }; // kword, colNum, rowNum, inTableNum
                inn.push_back(0);
            }

            lexStream.push_back(lex);
            curState = BEGIN_STATE;
            break;
        }

        case CONST: {
            std::string buff;
            Lexema lex;
            unsigned short int saveNInRow = numberInRow;

            while (coincidenceRegExprWithCh(c, RegExpression[2])) {
                buff += c;
                inFile.get(c);
                numberInRow++;

                if (inFile.eof()) {
                    c = ' ';
                    break;
                }
            }

            if ((!coincidenceRegExprWithCh(c, RegExpression[4]) && !coincidenceRegExprWithCh(c, RegExpression[5])) || (buff[0] == '0' && (buff.size() > 1))) {
                curState = ERROR;
                while (!coincidenceRegExprWithCh(c, RegExpression[4]) && !coincidenceRegExprWithCh(c, RegExpression[5])) {
                    buff += c;
                    inFile.get(c);
                }
                std::cerr << "Error \nlexem = \"" << buff << "\". (Uncorrect const)" << std::endl;
                break;
            }

            lex = { checkLexemClass("CONST"), 3, numberInColumn, saveNInRow, checkConst(buff) };
            lexStream.push_back(lex);
            curState = BEGIN_STATE;
            break;
        }

        case DIVIDER: {
            Lexema lex;
            lex = { checkLexemClass(std::string(1, c)), 1, numberInColumn, numberInRow, 0 };
            lexStream.push_back(lex);
            inFile.get(c);
            numberInRow++;
            curState = BEGIN_STATE;
            break;
        }
      
        case DSIGN: {
            Lexema lex;
            std::string buff;
            char c_prev = c;
            inFile.get(c);

            if (c_prev == '/' && c == '/') {
                while (c != '\n') inFile.get(c);
                curState = BEGIN_STATE;
                break;
            }
            else if (c_prev == '/' && c == '*') {
                        char help = c_prev;
                while (c_prev != '*' || c != '/') {
                    if (inFile.eof()) {
                        std::cerr << "Error in lexem = \"" << help << "\" (Uncorrect comment)" << std::endl;
                        curState = ERROR;
                        break;
                    }

                    c_prev = c;
                    inFile.get(c);
                }
                inFile.get(c);
                curState = BEGIN_STATE;
                break;
            }
            else if (c_prev == '+' && c == '+') {
                lex = { checkLexemClass(std::string("++")), 1, numberInColumn, numberInRow, 0};
                numberInRow++;
                inFile.get(c);
                lexStream.push_back(lex);
                curState = BEGIN_STATE;
                break;
            }
            else if (c_prev == '/') {
                lex = { checkLexemClass(std::string(1, c)), 1, numberInColumn, numberInRow, 0 };
                numberInRow++;
                lexStream.push_back(lex);
                curState = BEGIN_STATE;
                break;
            }
            else if (c != '=' && c_prev != '!') {
                lex = { checkLexemClass(std::string(1, c_prev)), 1, numberInColumn, numberInRow, 0 };
                numberInRow++;
                lexStream.push_back(lex);
                curState = BEGIN_STATE;
                break;
            }
            else if (c != '=' && c_prev == '!') {
                while (!coincidenceRegExprWithCh(c, RegExpression[4]) && !coincidenceRegExprWithCh(c, RegExpression[5])) {
                    buff += c;
                    inFile.get(c);
                }
                std::cerr << "Error in lexem = \"" << c_prev << "\" (Uncorrect symbol)" << std::endl;
                curState = ERROR;
                break;
            }
            
            if (c_prev == '=') {
                lex = { checkLexemClass("=="), 1, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '>') {
                lex = { checkLexemClass(">="), 1, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '<') {
                lex = { checkLexemClass("<="), 1, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '!') {
                lex = { checkLexemClass("!="), 1, numberInColumn, numberInRow, 0 };
            }
            inFile.get(c);
            numberInRow += 2;

            lexStream.push_back(lex);
            curState = BEGIN_STATE;
            break;
        }
        case ERROR: {
            std::cerr << "Line = " << numberInColumn << "\nPosition = " << numberInRow << std::endl << std::endl;
            Flag = false;
            break;

        }
        default:
            std::cerr << "Defautl Error!" << std::endl;
            break;
        }
    }
    return Flag;
}

void lexAnalyzer::displayTables() {
    std::cout << '#' << "\t" << std::setw(15) << std::left << "Lexem Class" << "\t" << "Lexem Type" << std::endl;
    for (int i = 0; i < lexStream.size(); i++) {
       
        std::cout << i << "\t" << std::setw(15) << std::left << lexemClass[lexStream[i].lexClass]
            << "\t" << lexemType[lexStream[i].lexType] << std::endl;
    }

    std::cout << '\n' << '#' << "\t" << "Name of Identifier" << std::endl;
    for (int i = 0; i < reprIdTable.size(); i++) {
        std::cout << i << "\t" << reprIdTable[i] << std::endl;
    }

    std::cout << '\n' << '#' << "\t" << "Constant" << std::endl;
    for (int i = 0; i < constTable.size(); i++) {
        std::cout << i << "\t" << constTable[i] << std::endl;
    }

    std::cout << '\n';
}

unsigned short int lexAnalyzer::checkReprId(const std::string& buff) {
    for (unsigned short int i = 0; i < reprIdTable.size(); i++) {
        if (reprIdTable[i] == buff) {
            return i;
        }
    }
    reprIdTable.push_back(buff);
    return reprIdTable.size() - 1;
}

unsigned short int lexAnalyzer::checkLexemClass(const std::string& buff) {
    for (unsigned short int i = 0; i < lexemClass.size(); i++) {
        if (lexemClass[i] == buff) {
            return i;
        }
    }
    lexemClass.push_back(buff);
    return lexemClass.size() - 1;
}

unsigned short int lexAnalyzer::checkConst(const std::string& buff) {
    for (unsigned short int i = 0; i < constTable.size(); i++) {
        if (constTable[i] == buff) {
            return i;
        }
    }
    constTable.push_back(buff);
    return constTable.size() - 1;
}