//Expr
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <map>
#include <variant>
#include <filesystem>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstring>
#include <algorithm>

using namespace std;

static void defineVisitor(ofstream& writer, string baseName, map<string, vector<string>> types) {
	writer << "  class " << baseName <<"Visitor {" << endl;
	writer << "    public: " << endl;
	const char* base = baseName.c_str();
	for (auto type : types) {
		string typeName = type.first; 
		writer << "    virtual void visit" + typeName + baseName +  "(const " + typeName + "* " + baseName + ") = 0;" <<endl; //make Basename lowercase somehow.
	}

	writer << "  };" << endl;
}

static void defineType(ofstream& writer, string baseName, string className, vector<string> fields) {
	//The base accept() method.
	writer << "  struct " + className + " : public " + baseName + " {" << endl;

	//Fields.
	for (size_t i = 0; i < fields.size(); i += 2) {
		writer << "    const " + fields[i] << " " << fields[i + 1] + ";" << endl << endl;
	}

	// Constructor
	writer << "    " + className + "(";

	for (size_t i = 0; i < fields.size(); i+=2) {

		writer << fields[i] <<" "<< fields[i + 1];
		if (i < fields.size() - 2) {
			writer << ", ";
		}
	}

	writer << ") : " << endl;
	

	for (size_t i = 1; i < fields.size(); i += 2) {
		writer << "       " + fields[i] + "{" + fields[i] + "}";
		if (i < fields.size() - 2) {
			writer << ",";
		}
	}

	writer << " {" << endl;

	// Store parameters in fields.
	
	writer <<"     }" << endl;
	// Visitor pattern.
	writer << "" << endl;
	writer << "    virtual void accept("<< baseName <<"Visitor* visitor) const override {" << endl;
	writer << "      return visitor->visit" + className + baseName + "(this);" << endl;
	writer << "    }" << endl;
	writer << "  };" << endl << endl;
}

static void defineAst(string outputDir, string baseName, map<string, vector<string>> types) {
	string path = outputDir + "/" + baseName + ".h";
	cout << "Creating file " << path << endl;
	ofstream writer(path); 
	writer <<"#pragma once"<< endl;
	writer << "#include \"Token.h\"" << endl; //DONT FORGET TO PUT THIS BACK TO "TOKEN.H" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	for (auto type : types) {
		writer << "struct " << type.first << ";"<< endl;
	}
	defineVisitor(writer, baseName, types);
	writer << "class " + baseName + " {" << endl;
	writer << "public: " << endl;
	writer << "  virtual void accept("<< baseName <<"Visitor* visitor) const = 0;" << endl << endl;
	writer << "};" << endl;
	// The AST classes
	for (auto type : types) {
		string className = type.first;
		vector<string> fields = type.second;
		defineType(writer, baseName, className, fields);
	}
	writer.close();
}


//Main Function
int main(int argc, char** argv) {
	cout << filesystem::current_path() << endl;

	if (argc != 2) {
		cout << "Usage: generate_ast <output directory>" << endl;	
	}
	string outputDir = argv[1];
	map<string, vector<string>> types{
		{"Binary"  , {"Expr*" ,"left", "Token", "oper", "Expr*", "right"} },
		{"Grouping", {"Expr*", "expression"}},
		{"Literal", {"LoxValue", "value"}},
		{"Unary", {"Token", "oper","Expr*", "right"}}
	};

	defineAst(outputDir, "Expr", types);
}