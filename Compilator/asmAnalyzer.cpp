#include "asmAnalyzer.h"

void asmAnalyzer :: analysis() {
	analysisCountID();
}

void asmAnalyzer::analysisCountID() {

	for (int i = 0; i < IdTable.size(); i++) {
		outFile << IdTable[i];
		std::string str = idType[i];
		if (str == "int") {
			outFile << " dd " << "?\n";
		}
		else if (str == "bool" || str == "char") {
			outFile << " db " << "?\n";
		}
	}

	for (int i = 0; i < countOperators; i++) {
		outFile << "TEMP" << i << " dd " << "?\n";
	}

}


void asmAnalyzer::analysisBody(const std::shared_ptr<Node>& node) {
	


	for (const auto& el : node->children)
	{
		//if (el->value == )
		printTree(el);
	}


}