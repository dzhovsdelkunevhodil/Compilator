#include "asmAnalyzer.h"

void asmAnalyzer :: analysis() {
	analysisCountID();
	analysisS();
}
//void asmAnalyzer::analysisBody(const std::shared_ptr<Node>& node);

void asmAnalyzer::analysisCountID() {

	for (int i = 0; i < IdTable.size(); i++) {
		outFile << IdTable[i];
		if (idType[i] == "int") {
			outFile << " dd " << "?\n";
		}
		else if (idType[i] == "bool" || idType[i] == "char") {
			outFile << " db " << "?\n";
		}
	}


	outFile << "\nTEMP" << " dd " << "0\n";
	outFile << "TEMPres" << " dd " << "0\n";

}


void asmAnalyzer::analysisS() {
	
	

	for (int i = 0; i < ParseTree->children.size(); i++) {
		//node = ParseTree->children[i];

		if (ParseTree->children[i]->value == "---Function---") {
			std::string nameFunc = "";
			for (const auto& el : ParseTree->children[i]->children)
			{
				if (el->value == "---IdFunc--") {
					nameFunc = el->children[0]->value;
					outFile << nameFunc << " proc\n";
				}
				else if (el->value == "-----Body-----") {
					flagLastOperation = 1;
					analysisBody(el);
					break;
				}
			}
			outFile << nameFunc << " endproc\n\n";
		}


		else if (ParseTree->children[i]->value == "-----Body-----") {
			outFile << "\nmain proc\n\n";
			analysisBody(ParseTree->children[i]);

		}

		else if (ParseTree->children[i]->value == "----Library---") {
			
			outFile << "\nlibrary ";
			if (ParseTree->children[i]->children[3]->value == "stdlib") {
				outFile << "stdlib\n\n";
			}
			else if (ParseTree->children[i]->children[3]->value == "stdlib") {
				outFile << "vector\n\n";
			}
			else if (ParseTree->children[i]->children[3]->value == "stdlib") {
				outFile << "iostream\n\n";
			}
		}


	}
	
	outFile << "\nmain endp\n";
}


void asmAnalyzer::analysisBody(const std::shared_ptr<Node>& node) {


	for (const auto& el : node->children) {
		if (el->value == "---Operator---") {

			if (el->children[0]->value == "--Assignment--") {
				std::string idAssigm = "";
				std::string res = "";

				for (const auto& elAssigment : el->children[0]->children) {
					if (elAssigment->value == "-------ID-------") {
						idAssigm = elAssigment->children[0]->value;
						if (flagLastOperation == 1) lastOperation = idAssigm;
					}
					else if (elAssigment->value == "--Expression--") {
						
						res += analysisExpression(elAssigment);
						break;
					}
				}
				outFile << "mov " << idAssigm << " , " << res << '\n';
			}
			else if (el->children[0]->value == "----Return----") {
				outFile << "push " << lastOperation << '\n';

				outFile << "ret\n";
			}
			else if (el->children[0]->value == "---Increase---") {
				if (el->children[0]->children[0]->value == "-------ID-------") {
					outFile << "inc " << el->children[0]->children[0]->children[0]->value<<'\n';
				}
			}
			else if (el->children[0]->value == "-----Loop-----") {

				outFile << "circle: \n";
				for (const auto& elLoop : el->children[0]->children) {
					if (elLoop->value == "-----Body-----") {
						analysisBody(elLoop);
						//break;
					}
					if (elLoop->value == "---Condition--") {
						outFile << "cmp " << elLoop->children[0]->children[0]->value << " , 0" << '\n';
						
						outFile << "jnz circle\n";
					}
				}
			}
		}
	}
}


std::string asmAnalyzer::analysisExpression(const std::shared_ptr<Node>& node) {

	///////////////////////////////////////////////////////
	for (int it = 0; it < node->children.size();it++) {
		if (node->children.size() == 1) {
			return analysisIDOrCallF(node->children[it]);
		}
		/////////////////////////////////////////////////

		else if (node->children.size() == 3) {
			int f = 0;
			for (const auto& elem : node->children) {
				if (elem->value == "--Expression--") {
					if (elem->children.size() != 1) {
						f = 1;
						break;
					}
				}
			}

			if (f == 0) {
				std::string res = "TEMPres";//*TEMPIDTable.end();
				//TEMPIDTable.pop_back();
				if (node->children[it + 1]->value == "+") {
					//res += analysisExpression(node->children[it]);
					outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					outFile << "add " << res << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;
				}
				else if (node->children[it + 1]->value == "-") {
					//res += analysisExpression(node->children[it]);
					outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					outFile << "sub " << res << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;

				}
				else if (node->children[it + 1]->value == "*") {
					//res += analysisExpression(node->children[it]);
					outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					outFile << "mul " << res << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;

				}

				return res;
			}

			else if (f == 1) {

				std::string res = "TEMP";//*TEMPIDTable.end();
				std::string allres = "TEMPres";
				//TEMPIDTable.pop_back();
				if (node->children[it + 1]->value == "+") {
					//res += analysisExpression(node->children[it]);
					if (node->children[it + 2]->children[1]->value == "+") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "add " << res << " , " << analysisIDOrCallF(node->children[it + 2] ->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "*") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "mul " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "-") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "sub " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					outFile << "mov " << allres << " , " << res << '\n';
					outFile << "add " << allres << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;
				}
				else if (node->children[it + 1]->value == "-") {
					//res += analysisExpression(node->children[it]);
					if (node->children[it + 2]->children[1]->value == "+") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "add " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "*") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "mul " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "-") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "sub " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					outFile << "mov " << allres << " , " << res << '\n';
					outFile << "sub " << allres << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;

				}
				else if (node->children[it + 1]->value == "*") {
					//res += analysisExpression(node->children[it]);
					if (node->children[it + 2]->children[1]->value == "+") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "add " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "*") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "mul " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					else if (node->children[it + 2]->children[1]->value == "-") {
						outFile << "mov " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[2]->children[0]) << '\n';
						outFile << "sub " << res << " , " << analysisIDOrCallF(node->children[it + 2]->children[0]) << '\n';
					}
					outFile << "mov " << allres << " , " << res << '\n';
					outFile << "mul " << allres << " , " << analysisIDOrCallF(node->children[it]) << '\n';
					//return res;

				}
				return allres;

			}
		}
	}
}



std::string asmAnalyzer::analysisIDOrCallF(const std::shared_ptr<Node>& node) {
	
			if ((node->value == "-CallFunction-")) {
				std::string nameFunc = "";
				for (int l = 0; l < node->children.size(); l++) {
					if (node->children[l]->value == "---IdFunc--") {
						nameFunc = node->children[l]->children[0]->value;
					}
					else if (node->children[l]->value == "(") {
						l++;
						while (node->children[l]->value != ")") {
							if (node->children[l]->value == "-------ID-------") {
								outFile << "push " << node->children[l]->children[0]->value << '\n';
								//parameter = elExpression->children[l]->children[0]->value;
							}
							l++;
						}
						break;
					}
				}
				outFile << "call " << nameFunc << '\n';


				return  "res";
			}

			else if ((node->value == "-------ID-------" || node->value == "-----CONST----")) {
				return node->children[0]->value;
			}	
}