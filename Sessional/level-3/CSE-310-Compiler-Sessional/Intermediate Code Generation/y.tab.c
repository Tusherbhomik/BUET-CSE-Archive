/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "2005046.y"

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<vector>
#include "SymbolInfo.h"
using namespace std;
string workWithCompoundStatement(SymbolInfo *root);
string workWithCompoundStatement(SymbolInfo *root);
string workWithStatements(SymbolInfo *root);
string workWithStatement(SymbolInfo *root);
string workWithExpression(SymbolInfo *root);
string workWithExpressionStatement(SymbolInfo *root);
string workWithLogicExpression(SymbolInfo *root);
string workWithDeclarationList(SymbolInfo *root);
string workWithVarDeclaration(SymbolInfo *root);
string workWithVariable(SymbolInfo *root);
string workWithRel(SymbolInfo *root);
int yyparse(void);
int yylex(void);
void yyerror(const char *s);

extern FILE *yyin;

FILE *logout,*input,*parseTree,*assembly;
SymbolTable *table;
int offsetCount=0;
int levelCount=0;

string newLineProc = "NEWLINE PROC\n\tPUSH AX\n\tPUSH DX\n\tMOV AH,2\n\tMOV DL,CR\n\tINT 21H\n\tMOV AH,2\n\tMOV DL,LF\n\tINT 21H\n\tPOP DX\n\tPOP AX\n\tRET\nNEWLINE ENDP\n";

string printOutputProc = "PRINTNUMBER PROC  ;PRINT WHAT IS IN AX\n\tPUSH AX\n\tPUSH BX\n\tPUSH CX\n\tPUSH DX\n\tPUSH SI\n\tLEA SI,NUMBER\n\tMOV BX,10\n\tADD SI,4\n\tCMP AX,0\n\tJNGE NEGATE\n\tPRINT:\n\tXOR DX,DX\n\tDIV BX\n\tMOV [SI],DL\n\tADD [SI],'0'\n\tDEC SI\n\tCMP AX,0\n\tJNE PRINT\n\tINC SI\n\tLEA DX,SI\n\tMOV AH,9\n\tINT 21H\n\tPOP SI\n\tPOP DX\n\tPOP CX\n\tPOP BX\n\tPOP AX\n\tRET\n\tNEGATE:\n\tPUSH AX\n\tMOV AH,2\n\tMOV DL,'-'\n\tINT 21H\n\tPOP AX\n\tNEG AX\n\tJMP PRINT\nPRINTNUMBER ENDP\n";

string opCode(string code){
	string opcode="";
	if(code=="<")opcode="JL";
	else if(code=="<=") opcode="JLE";
	else if(code==">") opcode="JG";
	else if(code==">=") opcode="JGE";
	else if(code=="!=") opcode="JNE";
	else if(code=="==") opcode="JE";	
	return opcode;
}

string header = ";-------\n;\n;-------\n.MODEL SMALL\n.STACK 1000H\n.DATA\n\tCR EQU 0DH\n\tLF EQU 0AH\n\tNUMBER DB \"00000$\"\n";

int createOffset(){
	return offsetCount+=2;
}
string type="";
vector<SymbolInfo*> variableList;

vector<SymbolInfo*> declaredButNotInserted;
vector<SymbolInfo*> funcDefVarList;
map<string,int> dummySymbolTable;
extern int line_count;
void printMap(const std::map<std::string, int>& symbolTable) {
    std::cout << "Symbol Table:\n";
    
    for (const auto& entry : symbolTable) {
        std::cout << entry.first << " : " << entry.second << '\n';
    }
}
SymbolInfo *createNode(string type, string value)
{
	SymbolInfo *node = new SymbolInfo(value, type);
	return node;
}

SymbolInfo *resolveSymbol(SymbolInfo *idNode)
{
	if (idNode == nullptr)
	{
		return nullptr;
	}
	if (idNode->symbol == nullptr)
	{
		idNode->symbol = table->lookUp(idNode->getName());
	}
	return idNode->symbol;
}

bool isGlobalSymbol(SymbolInfo *idNode)
{
	SymbolInfo *resolved = resolveSymbol(idNode);
	return resolved != nullptr && resolved->global;
}

void addChild(SymbolInfo *parent, SymbolInfo *child)
{
	parent->getChildren().push_back(child);
}
SymbolInfo *rootOfPareseTree;
void printParseTree(SymbolInfo *node, int depth)
{
	if (node == nullptr)
	{
		return;
	}
	for (int i = 0; i < depth; i++)
	{
		fprintf(parseTree, " ");
	} // indentation

	if (node->getType() == "rule")
	{
		// fprintf(parseTree,"Yes");
		fprintf(parseTree, "%s  ", node->getName().c_str());
	}
	else
	{
		fprintf(parseTree, "%s : %s    ", node->getType().c_str(), node->getName().c_str());
	}
	if (node->getType() != "rule")
	{
		fprintf(parseTree, "<Line: %d>\n", node->getStartLine());
	}
	else
	{
		fprintf(parseTree, "<Line: %d-%d>\n", node->getStartLine(), node->getEndLine());
	}
	for (SymbolInfo *child : node->getChildren())
	{
		printParseTree(child, depth + 1);
	}
}
void getUnits(SymbolInfo *root, vector<SymbolInfo *> &v)
{
	if (root->getName() == "program : unit")
	{
		v.push_back(root->getChildren()[0]);
	}
	else if (root->getName() == "program : program unit")
	{
		getUnits(root->getChildren()[0], v);
		v.push_back(root->getChildren()[1]);
	}
}
string analyzeList(SymbolInfo *root)
{
	vector<SymbolInfo *> children = root->getChildren();
	if (root->getName() == "declaration_list : declaration_list COMMA ID")
	{
		string temp = analyzeList(children[0]);
		return temp + '\t' + children[2]->getName() + " DW 1 DUP (0000H)\n";
	}
	if (root->getName() == "declaration_list : ID")
	{
		return '\t' + children[0]->getName() + " DW 1 DUP (0000H)\n";
	}
	if (root->getName() == "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE")
	{
		string temp = analyzeList(children[0]);
		return temp + '\t' + children[2]->getName() + " DW " + children[4]->getName() + " DUP (0000H)\n";
	}
	if (root->getName() == "declaration_list : ID LSQUARE CONST_INT RSQUARE")
	{
		return '\t' + children[0]->getName() + " DW " + children[2]->getName() + " DUP (0000H)\n";
	}
	return "";
}
string declareVar(SymbolInfo *root)
{
	return analyzeList(root->getChildren()[1]);
}
string workWithVariable(SymbolInfo *root){
	if(root->getName()=="variable : ID"){
		return "";

	}else if(root->getName()=="variable : ID LSQUARE expression RSQUARE"){
		string s= workWithExpression(root->getChildren()[2]);
		s+="\tPUSH AX\n";
		return s;
	}
	return "";
}

string workWithFactor(SymbolInfo *root){

	if(root->getName()=="factor : CONST_INT"){
		return "\tMOV AX, "+root->getChildren()[0]->getName()+"\n";
	}
	else if(root->getName()=="factor  : variable"){
		//SymbolInfo *temp=table->lookUp(root->getChildren()[0]->getChildren()[0]->getName());
		SymbolInfo *idNode = root->getChildren()[0]->getChildren()[0];
		if(isGlobalSymbol(idNode)){
			if(root->getChildren()[0]->getChildren().size()==1){
			return "\t MOV AX,"+root->getChildren()[0]->getChildren()[0]->getName()+"\n";
			}
			else{
				// string temp1="";
				string temp1=workWithVariable(root->getChildren()[0]);
				//temp1+="\tPUSH DX\n";
				temp1+="\tPOP BX\n";
				temp1+="\tMOV AX,2\n";
				temp1+="\tMUL BX\n";
				temp1+="\tMOV BX,AX\n";
				//temp1+="\tPOP AX\n";
				temp1+="\tMOV AX,"+root->getChildren()[0]->getChildren()[0]->getName()+"[BX]\n";
				return  temp1;
			}
		}
		else{
			string varName=root->getChildren()[0]->getChildren()[0]->getName();
			if(root->getChildren()[0]->getChildren().size()==1){
				return "\tMOV AX,[BP"+to_string(dummySymbolTable[varName])+"]\n";
			}
			else{
				int base=dummySymbolTable[varName];
				string temp1=workWithVariable(root->getChildren()[0]);
				temp1+="\tPOP BX\n";        // BX = index
				temp1+="\tMOV AX, 2\n";
				temp1+="\tMUL BX\n";         // AX = 2*index
				temp1+="\tNEG AX\n";         // AX = -2*index
				temp1+="\tADD AX, "+to_string(base)+"\n";  // AX = base - 2*index
				temp1+="\tMOV SI, AX\n";     // SI = effective offset
				temp1+="\tMOV AX, [BP+SI]\n"; // load element
				return temp1;
			}
		}
		
	}
	else if(root->getName()=="factor : variable INCOP"){
		string temp1="";
		SymbolInfo *idNode = root->getChildren()[0]->getChildren()[0];
		if(isGlobalSymbol(idNode)){
			temp1+= "\t MOV AX,"+root->getChildren()[0]->getChildren()[0]->getName()+"\n";
		}else{
			temp1+= "\t MOV AX,[BP"+to_string(dummySymbolTable[root->getChildren()[0]->getChildren()[0]->getName()])+"]\n";
		} 

		temp1+="\tINC AX\n";
		if(isGlobalSymbol(idNode)){
			temp1+= "\t MOV "+root->getChildren()[0]->getChildren()[0]->getName()+",AX \n";
		}else{
			temp1+= "\t MOV [BP"+to_string(dummySymbolTable[root->getChildren()[0]->getChildren()[0]->getName()])+"],AX\n";
		} 
		return temp1;
	}
	else if(root->getName()=="factor : variable DECOP"){
		string temp1="";
		SymbolInfo *idNode = root->getChildren()[0]->getChildren()[0];
		if(isGlobalSymbol(idNode)){
			temp1+= "\t MOV AX,"+root->getChildren()[0]->getChildren()[0]->getName()+"\n";
		}else{
			temp1+= "\t MOV AX,[BP"+to_string(dummySymbolTable[root->getChildren()[0]->getChildren()[0]->getName()])+"]\n";
		} 

		temp1+="\tDEC AX\n";
		if(isGlobalSymbol(idNode)){
			temp1+= "\t MOV "+root->getChildren()[0]->getChildren()[0]->getName()+",AX \n";
		}else{
			temp1+= "\t MOV [BP"+to_string(dummySymbolTable[root->getChildren()[0]->getChildren()[0]->getName()])+"],AX\n";
		} 
		return temp1;
	}

	else if(root->getName()=="factor : LPAREN expression RPAREN"){
		string temp=workWithExpression(root->getChildren()[1]);
		return temp;
	}
	return "";
}
string workWithUnaryExpression(SymbolInfo *root){
	if(root->getName()=="unary_expression : factor"){
		return workWithFactor(root->getChildren()[0]);
	}else if(root->getName()=="unary_expression : ADDOP unary_expression"){
		string temp=workWithUnaryExpression(root->getChildren()[1]);
		if(root->getChildren()[0]->getName()=="-"){
			temp+="\tNEG AX\n";
		}
		return temp;
	}else if(root->getName()=="unary_expression : NOT unary_expression"){
		string done="L"+to_string(levelCount++);
		string temp=workWithUnaryExpression(root->getChildren()[1]);
		temp+="\tCMP AX, 0\n";
		temp+="\tMOV AX, 0\n";
		temp+="\tJNE "+done+"\n";
		temp+="\tMOV AX, 1\n";
		temp+=done+":\n";
		return temp;
	}
	return "";
}
string workWithTerm(SymbolInfo * root){
	if(root->getName()=="term :  unary_expression"){
		return workWithUnaryExpression(root->getChildren()[0]);
	}
	else if(root->getName()=="term : term MULOP unary_expression"){
		string temp1=workWithUnaryExpression(root->getChildren()[2]);
		temp1+="\tPUSH AX\n";
		string temp2=workWithTerm(root->getChildren()[0]);
		temp2+="\tPOP CX\n";
		if(root->getChildren()[1]->getName()=="*"){
			temp2+="\tCWD\n";
			temp2+="\tMUL CX\n";
		}
		else if(root->getChildren()[1]->getName()=="/"){
			temp2+="\tCWD\n";
			temp2+="\tDIV CX\n\tMOV CX,AX\n";

		}
		else if(root->getChildren()[1]->getName()=="%"){
			temp2+="\tCWD\n";
			temp2+="\tDIV CX\n\tPUSH DX\n\tPOP AX\n";
		}
		return temp1+temp2;
	}
	else {
		return "";
	}

}
string workWithSimpleExpression(SymbolInfo *root){
	if(root->getName()=="simple_expression : term"){
		return workWithTerm(root->getChildren()[0]);
	}
	else if(root->getName()=="simple_expression : simple_expression ADDOP term"){
		string temp1=workWithTerm(root->getChildren()[2]);
		temp1+="\tPUSH AX\n";
		string temp2=workWithSimpleExpression(root->getChildren()[0]);
		if(root->getChildren()[1]->getName()=="+"){
			temp2+="\tPOP DX\n\tADD AX ,DX\n";
		}else {
			temp2+="\tPOP DX\n\tSUB AX ,DX\n";
		}
		return temp1+temp2;
	}
	return "";
}
string workWithRel(SymbolInfo *root){
	if(root->getName()=="rel_expression  : simple_expression"){
		return workWithSimpleExpression(root->getChildren()[0]);
	}else if(root->getName()=="rel_expression  : simple_expression RELOP simple_expression"){

		if(root->isCond){
			string temp1=workWithSimpleExpression(root->getChildren()[2]);
			temp1+="\tPUSH AX\n";
			string temp2=workWithSimpleExpression(root->getChildren()[0]);
			temp2+="\tPOP DX\n";

			string op=opCode(root->getChildren()[1]->getName());
			temp2+="\tCMP AX,DX\n";
			//temp2+="\tCMP AX,DX\n";
			temp2+="\t"+op+" "+root->truelevel+"\n";
			temp2+="\tJMP "+root->falselevel+"\n";
			temp2+=root->truelevel+":\n";
			return temp1+temp2;
		}


	else{
		string levelone="L"+to_string(levelCount++);
		string levelzero="L"+to_string(levelCount++);
		
		string nextLevel="L"+to_string(levelCount++);
		root->nextlevel=nextLevel;
		//cout<<root->nextlevel<<endl;
		
		string temp1=workWithSimpleExpression(root->getChildren()[2]);
		temp1+="\tPUSH AX\n";
		string temp2=workWithSimpleExpression(root->getChildren()[0]);
			temp2+="\tPOP DX\n";
		string op=opCode(root->getChildren()[1]->getName());
		temp2+="\tCMP AX,DX\n";
		temp2+="\t"+op+" "+levelone+"\n";
		temp2+="\tJMP "+levelzero+"\n";
		temp2+=levelone+":\n"+"\tMOV AX,1\n\tJMP "+nextLevel+"\n";
		temp2+=levelzero+":\n"+"\tMOV AX,0\n"+nextLevel+":\n";
		return temp1+temp2;
	}
	}
	return "";
}
string workWithLogicExpression(SymbolInfo *root){
	
	if(root->getName()=="logic_expression     : rel_expression"){
		root->getChildren()[0]->isCond=root->isCond;
		root->getChildren()[0]->truelevel=root->truelevel;
		root->getChildren()[0]->falselevel=root->falselevel;
		root->getChildren()[0]->nextlevel=root->nextlevel;
		return workWithRel(root->getChildren()[0]);
	}else if(root->getName()=="logic_expression : rel_expression LOGICOP rel_expression"){

	
		if(root->isCond){
			string temp1= workWithRel(root->getChildren()[0]);
			temp1+="\tCMP AX,0\n";

			if(root->getChildren()[1]->getName()=="||"){
			
			
			string temp2= workWithRel(root->getChildren()[2]);
			temp1+="\tJE "+root->truelevel+"\n";
			temp1+="\tJMP "+root->getChildren()[2]->nextlevel+"\n";
			temp1+=root->truelevel+":\n";
			string a="L"+to_string(levelCount++);
			temp1+="\tCMP AX, 0\n";
			temp1+="\tJNE "+a+"\n";
			temp1+="\tJMP "+root->falselevel+"\n";
			temp1+=a+":\n";

			return temp1+temp2;
			
			}
			if(root->getChildren()[1]->getName()=="&&"){

				string temp2= workWithRel(root->getChildren()[2]);
			temp1+="\tJNE "+root->truelevel+"\n";
			temp1+="\tJMP "+root->getChildren()[2]->nextlevel+"\n";
			temp1+=root->truelevel+":\n";
			string a="L"+to_string(levelCount++);
			string p="L"+to_string(levelCount++);
			temp1+="\tCMP AX, 0\n";
			temp1+="\tJNE "+a+"\n";
			temp1+="\tJMP "+root->falselevel+"\n";
			temp1+=a+":\n";
			temp2+="\tCMP AX,1\n";
			temp2+="\tJE "+p+"\n";
			temp2+="\tJMP "+root->falselevel+"\n";
			temp2+=p+":\n";

			
			
			return temp1+temp2;



			}


		}




	else{

		string trueLevel="L"+to_string(levelCount++);
		string falseLevel="L"+to_string(levelCount++);
		string levelOne="L"+to_string(levelCount++);
		string levelZero="L"+to_string(levelCount++);

		string temp1=workWithRel(root->getChildren()[0]);
		temp1+="\tCMP AX ,0\n";
		if(root->getChildren()[1]->getName()=="||"){
			temp1+="\tJNE "+levelOne+"\n";
			temp1+="\tJMP "+falseLevel+"\n";
			temp1+=levelOne+":\n\tMOV AX,1\n\tJMP "+trueLevel+"\n";
			temp1+=falseLevel+":\n";
			string temp2=workWithRel(root->getChildren()[2]);
			temp2+="\tCMP AX ,0\n";
			temp2+="\tJNE "+levelOne+"\n";
			temp2+="\tJMP "+levelZero+"\n";
			temp2+=levelZero+":\n\tMOV AX,0\n\tJMP "+trueLevel+"\n";
			temp2+=trueLevel+":\n";
			return temp1+temp2;
		}
		else if(root->getChildren()[1]->getName()=="&&")
		{
			temp1+="\tJE "+levelZero+"\n";
			temp1+="\tJMP "+trueLevel+"\n";
			temp1+=trueLevel+":\n";
			string temp2=workWithRel(root->getChildren()[2]);
			temp2+="\tCMP AX,0\n";
			temp2+="\tJE "+levelZero+"\n";
			temp2+="\tJMP "+levelOne+"\n";
			temp2+=levelOne+":\n";
			temp2+="\tMOV AX,1\n";
			temp2+="\tJMP "+falseLevel+"\n";
			temp2+=levelZero+":\n";
			temp2+="\tMOV AX,0\n";
			temp2+=falseLevel+":\n";
			return temp1+temp2;
		}
	}
		
	}else {
		return "";
	}
	return "";
}
string workWithExpression(SymbolInfo *root){
	if(root->getName()=="expression : variable ASSIGNOP logic_expression"){
		string temp1=workWithLogicExpression(root->getChildren()[2]);
		temp1+="\tPUSH AX\n";
		// SymbolInfo *temp=table->lookUp(root->getChildren()[0]->getChildren()[0]->getName());
		// if(temp==nullptr)cout<<"NULL Paisi"<<endl;
		SymbolInfo *idNode = root->getChildren()[0]->getChildren()[0];
		if(isGlobalSymbol(idNode)){
				if(root->getChildren()[0]->getChildren().size()==1){
						temp1+="\tPOP DX\n";
					temp1+="\tMOV "+root->getChildren()[0]->getChildren()[0]->getName()+" ,DX\n";
				}
				else{
					temp1+=workWithVariable(root->getChildren()[0]);
					temp1+="\tPOP BX\n";
					//temp1+="\tPUSH AX\n";
					temp1+="\tMOV AX,2\n";
					temp1+="\tMUL BX\n";
					temp1+="\tMOV BX,AX\n";
					temp1+="\tPOP AX\n";
					temp1+="\tMOV "+root->getChildren()[0]->getChildren()[0]->getName()+"[BX], AX\n";
				}
		}
		else{
			string varName=root->getChildren()[0]->getChildren()[0]->getName();
			if(root->getChildren()[0]->getChildren().size()==1){
				temp1+="\tPOP DX\n";
				temp1+="\tMOV [BP"+to_string(dummySymbolTable[varName])+"], DX\n";
			}
			else{
				int base=dummySymbolTable[varName];
				// temp1 ends with PUSH AX (RHS value on stack)
				// now compute address of array element
				temp1+=workWithVariable(root->getChildren()[0]); // pushes index
				temp1+="\tPOP BX\n";         // BX = index
				temp1+="\tMOV AX, 2\n";
				temp1+="\tMUL BX\n";          // AX = 2*index
				temp1+="\tNEG AX\n";          // AX = -2*index
				temp1+="\tADD AX, "+to_string(base)+"\n"; // AX = base - 2*index
				temp1+="\tMOV SI, AX\n";      // SI = effective offset
				temp1+="\tPOP AX\n";          // AX = RHS value
				temp1+="\tMOV [BP+SI], AX\n"; // store
			}
		}
		return temp1;
	}
	else if(root->getName()=="expression : logic_expression"){
		root->getChildren()[0]->isCond=root->isCond;
		root->getChildren()[0]->truelevel=root->truelevel;
		root->getChildren()[0]->falselevel=root->falselevel;
		root->getChildren()[0]->nextlevel=root->nextlevel;
		return workWithLogicExpression(root->getChildren()[0]);
	}else {
		return "";
	}
	
}
string workWithExpressionStatement(SymbolInfo *root){
	if(root->getName()=="expression_statement :SEMICOLON") return "";
	return workWithExpression(root->getChildren()[0]);
}
string workWithDeclarationList(SymbolInfo *root){
	if(root->getName()=="declaration_list : ID LSQUARE CONST_INT RSQUARE"){
		int size=stoi(root->getChildren()[2]->getName());
		offsetCount-=2;                                   // offset of first element
		dummySymbolTable[root->getChildren()[0]->getName()]=offsetCount;
		offsetCount-=2*(size-1);                          // rest of elements
		return "\tSUB SP,"+ to_string(size*2)+"\n";
	}
	else if(root->getName()=="declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE"){
		string temp=workWithDeclarationList(root->getChildren()[0]);
		int size=stoi(root->getChildren()[4]->getName());
		offsetCount-=2;
		dummySymbolTable[root->getChildren()[2]->getName()]=offsetCount;
		offsetCount-=2*(size-1);
		return temp+"\tSUB SP,"+ to_string(size*2)+"\n";
	}
	else if(root->getName()=="declaration_list : ID"){
		offsetCount-=2;
		dummySymbolTable[root->getChildren()[0]->getName()]=offsetCount;
		return "\tSUB SP,2\n";
	}
	else if(root->getName()=="declaration_list : declaration_list COMMA ID"){
		string temp=workWithDeclarationList(root->getChildren()[0]);
		offsetCount-=2;
		dummySymbolTable[root->getChildren()[2]->getName()]=offsetCount;
		return temp+"\tSUB SP,2\n";
	}
	else {
		return "";
	}
}
string workWithVarDeclaration(SymbolInfo *root){
	return workWithDeclarationList(root->getChildren()[1]);
}
string workWithStatement(SymbolInfo *root){
	
	if(root->getName()=="statement :  var_declaration"){
		return workWithVarDeclaration(root->getChildren()[0]);

	}else if(root->getName()=="statement :  expression_statement"){
		return workWithExpressionStatement(root->getChildren()[0]);
	}else if(root->getName()=="statement : compound_statement"){
		return workWithCompoundStatement(root->getChildren()[0]);
	}else if(root->getName()=="statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement"){
		string s="L"+to_string(levelCount++);
		string mainlevel="L"+to_string(levelCount++);

		string temp1=workWithExpressionStatement(root->getChildren()[2]);
		temp1+=s+":\n";
		string temp2=workWithExpressionStatement(root->getChildren()[3]);
			string s2="L"+to_string(levelCount++);
		temp2+=s2+":\n";
		temp2+="\tCMP AX,0\n";
		temp2+="\tJE "+mainlevel+"\n";

		//temp2+=root->getChildren()[3]->truelevel+"yes:\n";
		string temp3=workWithStatement(root->getChildren()[6]);
	
		string temp4=workWithExpression(root->getChildren()[4]);
		temp4+="\tJMP "+s+"\n";
		temp4+=mainlevel+":\n";

		return temp1+temp2+temp3+temp4;

		
	}else if(root->getName()=="statement : IF LPAREN expression RPAREN statement"){
		string truelevel="L"+to_string(levelCount++);
		string falselevel=root->nextlevel;
		root->getChildren()[2]->isCond=true;
		root->getChildren()[2]->truelevel=truelevel;
		root->getChildren()[2]->falselevel=falselevel;
		root->getChildren()[2]->nextlevel=truelevel;
		string temp=workWithExpression(root->getChildren()[2]);

		string temp2=workWithStatement(root->getChildren()[4]);
		temp2+=root->nextlevel+":\n";
		return temp+temp2;

	}else if(root->getName()=="statement : IF LPAREN expression RPAREN statement ELSE statement"){
		string truelevel="L"+to_string(levelCount++);
		string falselevel=root->nextlevel;
		root->getChildren()[2]->isCond=true;
		root->getChildren()[2]->truelevel=truelevel;
		root->getChildren()[2]->falselevel=falselevel;
		root->getChildren()[2]->nextlevel=truelevel;
		string temp=workWithExpression(root->getChildren()[2]);
		root->getChildren()[4]->nextlevel=truelevel;
		string temp2=workWithStatement(root->getChildren()[4]);
		string newLevel="L"+to_string(levelCount++);
		temp2+="\tJMP "+newLevel+"\n";
		temp2+=root->nextlevel+":\n";
		root->getChildren()[2]->nextlevel=newLevel;
		string temp3=workWithStatement(root->getChildren()[6]);
		temp3+=newLevel+":\n";
		return temp+temp2+temp3;	
	}
	else if(root->getName()=="statement : WHILE LPAREN expression RPAREN statement"){
		string k="L"+to_string(levelCount++);
		string s2="L"+to_string(levelCount++);
		string mainlevel="L"+to_string(levelCount++);
		string s=k+":\n";
		s+=workWithExpression(root->getChildren()[2]);
		s+=s2+":\n";
		s+="\tCMP AX,0\n";
		s+="\tJE "+mainlevel+"\n";
		string p=workWithStatement(root->getChildren()[4]);
		p+="\tJMP "+k+"\n";
		p+=mainlevel+":\n";
		return s+p;
	}
	else if(root->getName()=="statement : PRINTLN LPAREN ID RPAREN SEMICOLON"){
		if(isGlobalSymbol(root->getChildren()[2])){
			return "\t MOV AX,"+root->getChildren()[2]->getName()+"\n"+"\tCALL PRINTNUMBER\n\tCALL NEWLINE\n";
		}else{
			return "\t MOV AX,[BP"+to_string(dummySymbolTable[root->getChildren()[2]->getName()])+"]\n"+"\tCALL PRINTNUMBER\n\tCALL NEWLINE\n";
		}
	}
	else if(root->getName()=="statement : RETURN expression SEMICOLON"){
		return workWithExpression(root->getChildren()[1]);
	}
	else {
		return "";
	}
}
string workWithStatements(SymbolInfo *root){
	
	if(root->getName()=="statements : statements statement"){
		string nextLevel1="L"+to_string(levelCount++);
		string nextLevel2="L"+to_string(levelCount++);
		root->getChildren()[0]->nextlevel=nextLevel1;
		string temp1=workWithStatements(root->getChildren()[0]);
		root->getChildren()[1]->nextlevel=nextLevel2;
		string temp2=workWithStatement(root->getChildren()[1]);
		return temp1+temp2;
	}else{
		string nextLevel="L"+to_string(levelCount++);
		root->getChildren()[0]->nextlevel=nextLevel;


		return workWithStatement(root->getChildren()[0]);
	}
}
string workWithCompoundStatement(SymbolInfo *root){
	if(root->getName()=="compound_statement : LCURL RCURL") return "";
	return workWithStatements(root->getChildren()[1]);
}

string workWithFuncDefinition(SymbolInfo * root){
	offsetCount=0;
	dummySymbolTable.clear();
	if(root->getName()=="func_definition : type_specifier ID LPAREN RPAREN compound_statement"){
		return workWithCompoundStatement(root->getChildren()[4]);
	}else if(root->getName()=="func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement"){
		return workWithCompoundStatement(root->getChildren()[5]);
	}else{
		return "";
	}
}
string traverseParseTree(SymbolInfo *root)
{
	bool codePrinted=false;
	string result = "";
	vector<SymbolInfo *> v;
	getUnits(root, v);	//saved in the v
	for (auto x : v)
	{
		if (x->getName() == "unit : var_declaration")
		{	
			result += declareVar(x->getChildren()[0]);
		}
	}
	result+=".CODE\n";
	//for func definition
	for (auto x : v)
	{
		if(x->getName()=="unit : func_definition"){
			string funcName=x->getChildren()[0]->getChildren()[1]->getName();
			result+=funcName+" PROC\n";
			if(funcName=="main"){
				result+="\tMOV AX, @DATA\n\tMOV DS, AX\n";
			}
			result+="\tPUSH BP\n\tMOV BP, SP\n";
			result+=workWithFuncDefinition(x->getChildren()[0]);
			result+="\tMOV SP, BP\n\tPOP BP\n";
			if(funcName=="main"){
				result+="\tMOV AX, 4CH\n\tINT 21H\n";
			}else{
				result+="\tRET\n";
			}
			result+=funcName+" ENDP\n";
			result+=newLineProc+printOutputProc;
		}
	}
	return result;
}
void workWithAssembly()
{
	fprintf(assembly, ".MODEL SMALL\n.STACK 1000H\n.Data\n\tCR EQU 0DH\n\tLF EQU 0AH\n\tnumber DB \"00000$\"\n");
	fprintf(assembly, "%s", traverseParseTree(rootOfPareseTree->getChildren()[0]).c_str());
}

#line 817 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    ID = 262,                      /* ID  */
    LPAREN = 263,                  /* LPAREN  */
    RPAREN = 264,                  /* RPAREN  */
    SEMICOLON = 265,               /* SEMICOLON  */
    LCURL = 266,                   /* LCURL  */
    RCURL = 267,                   /* RCURL  */
    COMMA = 268,                   /* COMMA  */
    INT = 269,                     /* INT  */
    FLOAT = 270,                   /* FLOAT  */
    VOID = 271,                    /* VOID  */
    LTHIRD = 272,                  /* LTHIRD  */
    CONST_INT = 273,               /* CONST_INT  */
    RTHIRD = 274,                  /* RTHIRD  */
    PRINTLN = 275,                 /* PRINTLN  */
    RETURN = 276,                  /* RETURN  */
    ASSIGNOP = 277,                /* ASSIGNOP  */
    LOGICOP = 278,                 /* LOGICOP  */
    RELOP = 279,                   /* RELOP  */
    ADDOP = 280,                   /* ADDOP  */
    MULOP = 281,                   /* MULOP  */
    CONST_FLOAT = 282,             /* CONST_FLOAT  */
    NOT = 283,                     /* NOT  */
    INCOP = 284,                   /* INCOP  */
    DECOP = 285,                   /* DECOP  */
    LOWER_THAN_ELSE = 286          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define ID 262
#define LPAREN 263
#define RPAREN 264
#define SEMICOLON 265
#define LCURL 266
#define RCURL 267
#define COMMA 268
#define INT 269
#define FLOAT 270
#define VOID 271
#define LTHIRD 272
#define CONST_INT 273
#define RTHIRD 274
#define PRINTLN 275
#define RETURN 276
#define ASSIGNOP 277
#define LOGICOP 278
#define RELOP 279
#define ADDOP 280
#define MULOP 281
#define CONST_FLOAT 282
#define NOT 283
#define INCOP 284
#define DECOP 285
#define LOWER_THAN_ELSE 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 747 "2005046.y"

	SymbolInfo* sym;

#line 936 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_FOR = 5,                        /* FOR  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_ID = 7,                         /* ID  */
  YYSYMBOL_LPAREN = 8,                     /* LPAREN  */
  YYSYMBOL_RPAREN = 9,                     /* RPAREN  */
  YYSYMBOL_SEMICOLON = 10,                 /* SEMICOLON  */
  YYSYMBOL_LCURL = 11,                     /* LCURL  */
  YYSYMBOL_RCURL = 12,                     /* RCURL  */
  YYSYMBOL_COMMA = 13,                     /* COMMA  */
  YYSYMBOL_INT = 14,                       /* INT  */
  YYSYMBOL_FLOAT = 15,                     /* FLOAT  */
  YYSYMBOL_VOID = 16,                      /* VOID  */
  YYSYMBOL_LTHIRD = 17,                    /* LTHIRD  */
  YYSYMBOL_CONST_INT = 18,                 /* CONST_INT  */
  YYSYMBOL_RTHIRD = 19,                    /* RTHIRD  */
  YYSYMBOL_PRINTLN = 20,                   /* PRINTLN  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_ASSIGNOP = 22,                  /* ASSIGNOP  */
  YYSYMBOL_LOGICOP = 23,                   /* LOGICOP  */
  YYSYMBOL_RELOP = 24,                     /* RELOP  */
  YYSYMBOL_ADDOP = 25,                     /* ADDOP  */
  YYSYMBOL_MULOP = 26,                     /* MULOP  */
  YYSYMBOL_CONST_FLOAT = 27,               /* CONST_FLOAT  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_INCOP = 29,                     /* INCOP  */
  YYSYMBOL_DECOP = 30,                     /* DECOP  */
  YYSYMBOL_LOWER_THAN_ELSE = 31,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_start = 33,                     /* start  */
  YYSYMBOL_program = 34,                   /* program  */
  YYSYMBOL_unit = 35,                      /* unit  */
  YYSYMBOL_func_declaration = 36,          /* func_declaration  */
  YYSYMBOL_func_definition = 37,           /* func_definition  */
  YYSYMBOL_parameter_list = 38,            /* parameter_list  */
  YYSYMBOL_compound_statement = 39,        /* compound_statement  */
  YYSYMBOL_40_1 = 40,                      /* $@1  */
  YYSYMBOL_41_2 = 41,                      /* $@2  */
  YYSYMBOL_var_declaration = 42,           /* var_declaration  */
  YYSYMBOL_type_specifier = 43,            /* type_specifier  */
  YYSYMBOL_declaration_list = 44,          /* declaration_list  */
  YYSYMBOL_statements = 45,                /* statements  */
  YYSYMBOL_statement = 46,                 /* statement  */
  YYSYMBOL_expression_statement = 47,      /* expression_statement  */
  YYSYMBOL_variable = 48,                  /* variable  */
  YYSYMBOL_expression = 49,                /* expression  */
  YYSYMBOL_logic_expression = 50,          /* logic_expression  */
  YYSYMBOL_rel_expression = 51,            /* rel_expression  */
  YYSYMBOL_simple_expression = 52,         /* simple_expression  */
  YYSYMBOL_term = 53,                      /* term  */
  YYSYMBOL_unary_expression = 54,          /* unary_expression  */
  YYSYMBOL_factor = 55,                    /* factor  */
  YYSYMBOL_argument_list = 56,             /* argument_list  */
  YYSYMBOL_arguments = 57                  /* arguments  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  120

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   760,   760,   774,   784,   793,   801,   809,   818,   840,
     863,   887,   912,   926,   937,   947,   956,   956,   968,   968,
     979,   989,   998,  1007,  1016,  1037,  1062,  1081,  1105,  1113,
    1122,  1130,  1138,  1146,  1160,  1171,  1185,  1197,  1210,  1220,
    1228,  1237,  1251,  1270,  1278,  1288,  1296,  1306,  1314,  1325,
    1333,  1343,  1352,  1362,  1371,  1380,  1388,  1396,  1407,  1417,
    1426,  1435,  1445,  1454,  1462,  1467,  1477
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "FOR",
  "WHILE", "ID", "LPAREN", "RPAREN", "SEMICOLON", "LCURL", "RCURL",
  "COMMA", "INT", "FLOAT", "VOID", "LTHIRD", "CONST_INT", "RTHIRD",
  "PRINTLN", "RETURN", "ASSIGNOP", "LOGICOP", "RELOP", "ADDOP", "MULOP",
  "CONST_FLOAT", "NOT", "INCOP", "DECOP", "LOWER_THAN_ELSE", "$accept",
  "start", "program", "unit", "func_declaration", "func_definition",
  "parameter_list", "compound_statement", "$@1", "$@2", "var_declaration",
  "type_specifier", "declaration_list", "statements", "statement",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-65)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-19)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      29,   -65,   -65,   -65,    17,    29,   -65,   -65,   -65,   -65,
      49,   -65,   -65,     2,     5,    45,    10,   -65,    60,    39,
      21,    65,    33,    59,   -65,    68,   -65,    75,    29,   -65,
     -65,    64,    89,    81,   -65,   -65,    80,    70,    98,   100,
     103,     4,    30,   -65,   -65,   110,    30,    30,   -65,    30,
     -65,   -65,    91,    63,   -65,   -65,     3,   111,   -65,    99,
      77,   101,   -65,   -65,   -65,   -65,   -65,    30,   105,    30,
      30,    30,   115,   118,   116,    90,   -65,   -65,   112,   -65,
     -65,    30,   -65,   -65,   -65,    30,    30,    30,    30,   119,
     105,   122,   -65,   125,   123,   120,   -65,   126,   -65,   -65,
     -65,   113,   101,   -65,    89,    30,    89,   -65,    30,   -65,
     127,   136,   132,   -65,   -65,   -65,    89,    89,   -65,   -65
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    21,    22,    23,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    26,     0,     0,     0,    20,     0,     0,
       0,    15,     0,    24,     9,    16,    11,     0,     0,    14,
      27,     0,     0,     0,     8,    10,    13,     0,     0,     0,
       0,    41,     0,    39,    59,     0,     0,     0,    60,     0,
      32,    30,     0,     0,    28,    31,    56,     0,    43,    45,
      47,    49,    51,    55,    19,    12,    25,     0,     0,     0,
      64,     0,     0,     0,     0,    56,    53,    54,    26,    17,
      29,     0,    61,    62,    40,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,    58,     0,    38,    44,
      46,    48,    50,    52,     0,     0,     0,    57,     0,    42,
       0,    34,     0,    36,    65,    37,     0,     0,    35,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -65,   -65,   -65,   137,   -65,   -65,   -65,    -5,   -65,   -65,
      11,     8,   -65,   -65,   -53,   -64,   -46,   -40,   -61,    58,
      61,    57,   -42,   -65,   -65,   -65
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    20,    50,    32,    33,
      51,    52,    14,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    93,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      80,    75,    72,    75,    90,    76,    74,    77,    10,    92,
      15,     9,    70,    10,    26,    17,     9,    11,    18,    16,
      99,    71,    35,    21,    75,    81,   105,    89,    22,    91,
      27,    95,    82,    83,    28,    75,    36,    41,    42,    75,
      75,    75,    75,     1,     2,     3,   103,   114,    44,    24,
      25,   111,    30,   113,    19,    47,    13,    48,    49,     1,
       2,     3,    75,   118,   119,   112,    38,    23,    39,    40,
      41,    42,    29,    43,    25,    79,    31,     1,     2,     3,
     -18,    44,    37,    45,    46,    34,    25,    65,    47,    66,
      48,    49,    38,    64,    39,    40,    41,    42,    78,    43,
      25,    86,    87,     1,     2,     3,    67,    44,    68,    45,
      46,    69,    41,    42,    47,    43,    48,    49,    73,    82,
      83,    84,    85,    44,    96,    97,    98,    88,   104,    16,
      47,   106,    48,    49,   107,   110,   108,   115,    87,   109,
     116,   117,    12,   100,   102,     0,     0,   101
};

static const yytype_int8 yycheck[] =
{
      53,    47,    42,    49,    68,    47,    46,    49,     0,    70,
       8,     0,     8,     5,    19,    10,     5,     0,    13,    17,
      81,    17,    27,    15,    70,    22,    90,    67,    18,    69,
       9,    71,    29,    30,    13,    81,    28,     7,     8,    85,
      86,    87,    88,    14,    15,    16,    88,   108,    18,    10,
      11,   104,    19,   106,     9,    25,     7,    27,    28,    14,
      15,    16,   108,   116,   117,   105,     3,     7,     5,     6,
       7,     8,     7,    10,    11,    12,    17,    14,    15,    16,
      12,    18,    18,    20,    21,    10,    11,     7,    25,    19,
      27,    28,     3,    12,     5,     6,     7,     8,     7,    10,
      11,    24,    25,    14,    15,    16,     8,    18,     8,    20,
      21,     8,     7,     8,    25,    10,    27,    28,     8,    29,
      30,    10,    23,    18,     9,     7,    10,    26,     9,    17,
      25,     9,    27,    28,     9,     9,    13,    10,    25,    19,
       4,     9,     5,    85,    87,    -1,    -1,    86
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    14,    15,    16,    33,    34,    35,    36,    37,    42,
      43,     0,    35,     7,    44,     8,    17,    10,    13,     9,
      38,    43,    18,     7,    10,    11,    39,     9,    13,     7,
      19,    17,    40,    41,    10,    39,    43,    18,     3,     5,
       6,     7,     8,    10,    18,    20,    21,    25,    27,    28,
      39,    42,    43,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    12,     7,    19,     8,     8,     8,
       8,    17,    49,     8,    49,    48,    54,    54,     7,    12,
      46,    22,    29,    30,    10,    23,    24,    25,    26,    49,
      47,    49,    50,    56,    57,    49,     9,     7,    10,    50,
      51,    52,    53,    54,     9,    47,     9,     9,    13,    19,
       9,    46,    49,    46,    50,    10,     4,     9,    46,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    36,    36,
      37,    37,    38,    38,    38,    38,    40,    39,    41,    39,
      42,    43,    43,    43,    44,    44,    44,    44,    45,    45,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    55,    55,    55,    55,
      55,    55,    55,    56,    56,    57,    57
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       6,     5,     4,     3,     2,     1,     0,     4,     0,     3,
       3,     1,     1,     1,     3,     6,     1,     4,     1,     2,
       1,     1,     1,     7,     5,     7,     5,     5,     3,     1,
       2,     1,     4,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     2,     2,     1,     1,     4,     3,     1,
       1,     2,     2,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: program  */
#line 761 "2005046.y"
{
    fprintf(logout,"start : program\n");
    (yyval.sym) = createNode("rule","start: program");
    rootOfPareseTree=(yyval.sym);
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    addChild((yyval.sym), (yyvsp[0].sym));
	table->printAllScopeTable();
    printParseTree((yyval.sym), 0);
    workWithAssembly();
}
#line 2063 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 775 "2005046.y"
{
    fprintf(logout,"program : program unit  \n");
    (yyval.sym) = createNode("rule","program : program unit");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

}
#line 2077 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 785 "2005046.y"
{
    fprintf(logout,"program : unit  \n");
    (yyval.sym) = createNode("rule","program : unit");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2089 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 794 "2005046.y"
{
    fprintf(logout,"unit : var_declaration  \n");
    (yyval.sym) = createNode("rule","unit : var_declaration");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2101 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 802 "2005046.y"
{
    fprintf(logout,"unit : func_declaration \n");
    (yyval.sym) = createNode("rule","unit : func_declaration");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2113 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 810 "2005046.y"
{
    fprintf(logout,"unit : func_definition  \n");
    (yyval.sym) = createNode("rule","unit : func_definition");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2125 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 819 "2005046.y"
{
    fprintf(logout,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n");
    (yyval.sym) = createNode("rule","func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
    addChild((yyval.sym), (yyvsp[-5].sym));
    addChild((yyval.sym), (yyvsp[-4].sym));
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-5].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

    SymbolInfo *s=new SymbolInfo((yyvsp[-4].sym)->getName(),(yyvsp[-5].sym)->getType());
    s->setIsFunc();
    table->insert(s);
    for(int i=0;i<variableList.size();i++)
    {
		s->addParam(variableList[i]);
    }
    variableList.clear();
}
#line 2151 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 841 "2005046.y"
{
    fprintf(logout,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
    (yyval.sym) = createNode("rule","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
    addChild((yyval.sym), (yyvsp[-4].sym));
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-4].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    
    SymbolInfo *s=new SymbolInfo((yyvsp[-3].sym)->getName(),(yyvsp[-4].sym)->getType());
    s->setIsFunc();
    table->insert(s);
    for(int i=0;i<variableList.size();i++)
    {
		s->addParam(variableList[i]);
    }
    variableList.clear();
}
#line 2176 "y.tab.c"
    break;

  case 10: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN compound_statement  */
#line 864 "2005046.y"
{
    fprintf(logout,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
    (yyval.sym) = createNode("rule","func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
    addChild((yyval.sym), (yyvsp[-5].sym));
    addChild((yyval.sym), (yyvsp[-4].sym));
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-5].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());


    SymbolInfo *s=new SymbolInfo((yyvsp[-4].sym)->getName(),(yyvsp[-5].sym)->getType());
    s->setDefine();
    s->setIsFunc();
    table->insert(s);//function name inserted in the symbol table
    for(int i=0;i<variableList.size();i++)
    {
		s->addParam(variableList[i]);
    }
    variableList.clear();
	}
#line 2204 "y.tab.c"
    break;

  case 11: /* func_definition: type_specifier ID LPAREN RPAREN compound_statement  */
#line 888 "2005046.y"
{
    fprintf(logout,"func_definition : type_specifier ID LPAREN RPAREN compound_statement \n");
    (yyval.sym) = createNode("rule","func_definition : type_specifier ID LPAREN RPAREN compound_statement");
    addChild((yyval.sym), (yyvsp[-4].sym));
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-4].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

    SymbolInfo *s=new SymbolInfo((yyvsp[-3].sym)->getName(),(yyvsp[-4].sym)->getType());//function name
    s->setIsFunc();
    s->setDefine();
    table->insert(s);
    // for(int i=0;i<variableList.size();i++)
    // {
    //   s->addParam(variableList[i]);
    //   cout<<"length"<<variableList.size()<<endl;
    // }
    variableList.clear();
}
#line 2231 "y.tab.c"
    break;

  case 12: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 913 "2005046.y"
{
    fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier ID \n");
    (yyval.sym) = createNode("rule","parameter_list  : parameter_list COMMA type_specifier ID");
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-3].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

    SymbolInfo *s=new SymbolInfo((yyvsp[0].sym)->getName(),type);
    variableList.push_back(s);
}
#line 2249 "y.tab.c"
    break;

  case 13: /* parameter_list: parameter_list COMMA type_specifier  */
#line 927 "2005046.y"
{
    fprintf(logout,"parameter_list  : parameter_list COMMA type_specifier  \n");
    (yyval.sym) = createNode("rule","parameter_list  : parameter_list COMMA type_specifier");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    
}
#line 2264 "y.tab.c"
    break;

  case 14: /* parameter_list: type_specifier ID  */
#line 938 "2005046.y"
{
    fprintf(logout,"parameter_list  : type_specifier ID \n");
    (yyval.sym) = createNode("rule","parameter_list  : type_specifier ID");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    variableList.push_back((yyvsp[0].sym));
	}
#line 2278 "y.tab.c"
    break;

  case 15: /* parameter_list: type_specifier  */
#line 948 "2005046.y"
        {
    fprintf(logout,"parameter_list  : type_specifier  \n");
    (yyval.sym) = createNode("rule","parameter_list  : type_specifier");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
	}
#line 2290 "y.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 956 "2005046.y"
                          {table->enterScope();}
#line 2296 "y.tab.c"
    break;

  case 17: /* compound_statement: LCURL $@1 statements RCURL  */
#line 957 "2005046.y"
{
    fprintf(logout,"compound_statement : LCURL statements RCURL \n");
    (yyval.sym) = createNode("rule","compound_statement : LCURL statements RCURL");
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-3].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    table->printAllScopeTable();
    table->exitScope();
}
#line 2312 "y.tab.c"
    break;

  case 18: /* $@2: %empty  */
#line 968 "2005046.y"
       {table->enterScope();}
#line 2318 "y.tab.c"
    break;

  case 19: /* compound_statement: LCURL $@2 RCURL  */
#line 969 "2005046.y"
{
    fprintf(logout,"compound_statement : LCURL RCURL \n");
    (yyval.sym) = createNode("rule","compound_statement : LCURL RCURL");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    table->printAllScopeTable();
    table->exitScope();
}
#line 2333 "y.tab.c"
    break;

  case 20: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 980 "2005046.y"
{
    fprintf(logout,"var_declaration : type_specifier declaration_list SEMICOLON  \n");
    (yyval.sym) = createNode("rule","var_declaration : type_specifier declaration_list SEMICOLON");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2347 "y.tab.c"
    break;

  case 21: /* type_specifier: INT  */
#line 990 "2005046.y"
{
    fprintf(logout,"type_specifier  : INT \n");
        (yyval.sym)= createNode("rule","type_specifier  : INT");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    type="INT";
}
#line 2360 "y.tab.c"
    break;

  case 22: /* type_specifier: FLOAT  */
#line 999 "2005046.y"
{
    fprintf(logout,"type_specifier  : FLOAT \n");
    (yyval.sym) = createNode("rule","type_specifier  : FLOAT");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    type="FLOAT";
}
#line 2373 "y.tab.c"
    break;

  case 23: /* type_specifier: VOID  */
#line 1008 "2005046.y"
{
    fprintf(logout,"type_specifier  : VOID\n");
    (yyval.sym) = createNode("rule","type_specifier  : VOID");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
    type="VOID";
}
#line 2386 "y.tab.c"
    break;

  case 24: /* declaration_list: declaration_list COMMA ID  */
#line 1017 "2005046.y"
{
    fprintf(logout,"declaration_list : declaration_list COMMA ID  \n");
    (yyval.sym) = createNode("rule","declaration_list : declaration_list COMMA ID");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

    SymbolInfo* symbolInfoPtr = table->lookUp((yyvsp[-2].sym)->getName());
    if(symbolInfoPtr==nullptr){
		if(table->getId()==1){
			(yyvsp[0].sym)->global=true;
		}
		
		table->insert((yyvsp[0].sym));
		
    }//changed

}
#line 2411 "y.tab.c"
    break;

  case 25: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 1038 "2005046.y"
{
    fprintf(logout,"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
    (yyval.sym) = createNode("rule","declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
    addChild((yyval.sym), (yyvsp[-5].sym));
    addChild((yyval.sym), (yyvsp[-4].sym));
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-5].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

    SymbolInfo* symbolInfoPtr = table->lookUp((yyvsp[-3].sym)->getName());
    if(symbolInfoPtr==nullptr){
		if(table->getId()==1){
			(yyvsp[-3].sym)->global=true;
			
		}
		(yyvsp[-5].sym)->arraySize=stoi((yyvsp[-3].sym)->getName());
		(yyvsp[-5].sym)->isArray=true;
    table->insert((yyvsp[-3].sym));
	
    }//changed
}
#line 2440 "y.tab.c"
    break;

  case 26: /* declaration_list: ID  */
#line 1063 "2005046.y"
{
	
    fprintf(logout,"declaration_list : ID \n");
        (yyval.sym) = createNode("rule","declaration_list : ID");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
	
    SymbolInfo* symbolInfoPtr = table->lookUp((yyvsp[0].sym)->getName());
    if(symbolInfoPtr==nullptr){
		if(table->getId()==1){
			(yyvsp[0].sym)->global=true;
		}
		
		table->insert((yyvsp[0].sym));
		
    }//changed
}
#line 2463 "y.tab.c"
    break;

  case 27: /* declaration_list: ID LTHIRD CONST_INT RTHIRD  */
#line 1082 "2005046.y"
{
    fprintf(logout,"declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
    (yyval.sym) = createNode("rule","declaration_list : ID LSQUARE CONST_INT RSQUARE");
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-3].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());


    SymbolInfo* symbolInfoPtr = table->lookUp((yyvsp[-3].sym)->getName());
    if(symbolInfoPtr==nullptr){
		if(table->getId()==1){
			(yyvsp[-3].sym)->global=true;

		}
		(yyvsp[-3].sym)->arraySize=stoi((yyvsp[-1].sym)->getName());
		(yyvsp[-3].sym)->isArray=true;
    table->insert((yyvsp[-3].sym));
    }//changed
	
	}
#line 2491 "y.tab.c"
    break;

  case 28: /* statements: statement  */
#line 1106 "2005046.y"
                                {
					fprintf(logout,"statements : statement  \n");
					(yyval.sym) = createNode("rule","statements : statement");
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2503 "y.tab.c"
    break;

  case 29: /* statements: statements statement  */
#line 1114 "2005046.y"
                                {
					fprintf(logout,"statements : statements statement \n");
					(yyval.sym) = createNode("rule","statements : statements statement");
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2516 "y.tab.c"
    break;

  case 30: /* statement: var_declaration  */
#line 1123 "2005046.y"
                                {
					fprintf(logout,"statement :  var_declaration \n");
					(yyval.sym) = createNode("rule","statement :  var_declaration");
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2528 "y.tab.c"
    break;

  case 31: /* statement: expression_statement  */
#line 1131 "2005046.y"
                                {
					fprintf(logout,"statement :  expression_statement \n");
					(yyval.sym) = createNode("rule","statement :  expression_statement");
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2540 "y.tab.c"
    break;

  case 32: /* statement: compound_statement  */
#line 1139 "2005046.y"
                                {
					fprintf(logout,"statement : compound_statement \n");
					(yyval.sym) = createNode("rule","statement : compound_statement");
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2552 "y.tab.c"
    break;

  case 33: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 1147 "2005046.y"
                                {
					fprintf(logout,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement \n");
					(yyval.sym) = createNode("rule","statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
					addChild((yyval.sym), (yyvsp[-6].sym));
					addChild((yyval.sym), (yyvsp[-5].sym));
					addChild((yyval.sym), (yyvsp[-4].sym));
					addChild((yyval.sym), (yyvsp[-3].sym));
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-6].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2570 "y.tab.c"
    break;

  case 34: /* statement: IF LPAREN expression RPAREN statement  */
#line 1161 "2005046.y"
                                {
					fprintf(logout,"statement : IF LPAREN expression RPAREN statement \n");(yyval.sym) = createNode("rule","statement : IF LPAREN expression RPAREN statement");
					addChild((yyval.sym), (yyvsp[-4].sym));
					addChild((yyval.sym), (yyvsp[-3].sym));
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-4].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2585 "y.tab.c"
    break;

  case 35: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 1172 "2005046.y"
                                {
					fprintf(logout,"statement : IF LPAREN expression RPAREN statement ELSE statement \n");
					(yyval.sym) = createNode("rule","statement : IF LPAREN expression RPAREN statement ELSE statement");
					addChild((yyval.sym), (yyvsp[-6].sym));
					addChild((yyval.sym), (yyvsp[-5].sym));
					addChild((yyval.sym), (yyvsp[-4].sym));
					addChild((yyval.sym), (yyvsp[-3].sym));
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-6].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2603 "y.tab.c"
    break;

  case 36: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 1186 "2005046.y"
                                {
					fprintf(logout,"statement : WHILE LPAREN expression RPAREN statement \n");
					(yyval.sym) = createNode("rule","statement : WHILE LPAREN expression RPAREN statement");
					addChild((yyval.sym), (yyvsp[-4].sym));
					addChild((yyval.sym), (yyvsp[-3].sym));
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-4].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
					}
#line 2619 "y.tab.c"
    break;

  case 37: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 1198 "2005046.y"
                                {//Id niye kaj kora lagbe
					fprintf(logout,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON \n");
					(yyval.sym) = createNode("rule","statement : PRINTLN LPAREN ID RPAREN SEMICOLON");addChild((yyval.sym), (yyvsp[-4].sym));
					addChild((yyval.sym), (yyvsp[-3].sym));
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-4].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
					SymbolInfo *temp=table->lookUp((yyvsp[-2].sym)->getName());
					(yyvsp[-2].sym)->symbol=temp;
				}
#line 2636 "y.tab.c"
    break;

  case 38: /* statement: RETURN expression SEMICOLON  */
#line 1211 "2005046.y"
                                {
					fprintf(logout,"statement : RETURN expression SEMICOLON \n");
					(yyval.sym) = createNode("rule","statement : RETURN expression SEMICOLON");
					addChild((yyval.sym), (yyvsp[-2].sym));
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2650 "y.tab.c"
    break;

  case 39: /* expression_statement: SEMICOLON  */
#line 1221 "2005046.y"
                                {
					fprintf(logout,"expression_statement :SEMICOLON \n");
					(yyval.sym) = createNode("rule","expression_statement :SEMICOLON");
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2662 "y.tab.c"
    break;

  case 40: /* expression_statement: expression SEMICOLON  */
#line 1229 "2005046.y"
                                {
					fprintf(logout,"expression_statement :expression SEMICOLON \n");
					(yyval.sym) = createNode("rule","expression_statement :expression SEMICOLON");
					addChild((yyval.sym), (yyvsp[-1].sym));
					addChild((yyval.sym), (yyvsp[0].sym));
					(yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
					(yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
				}
#line 2675 "y.tab.c"
    break;

  case 41: /* variable: ID  */
#line 1238 "2005046.y"
{
    // if(table->lookUp($1)==NULL){
    //   table->insert($1);
    // }//changed
    fprintf(logout,"variable : ID  \n");
    (yyval.sym) = createNode("rule","variable : ID");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
	SymbolInfo *symbol=table->lookUp((yyvsp[0].sym)->getName());
	(yyvsp[0].sym)->symbol=symbol;

}
#line 2693 "y.tab.c"
    break;

  case 42: /* variable: ID LTHIRD expression RTHIRD  */
#line 1252 "2005046.y"
{
    // if(table->lookUp($1)==NULL){
    //   table->insert($1);
    // }//changed

    fprintf(logout,"variable : ID LSQUARE expression RSQUARE \n");
    (yyval.sym) = createNode("rule","variable : ID LSQUARE expression RSQUARE");
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-3].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
	SymbolInfo *symbol=table->lookUp((yyvsp[-3].sym)->getName());
	(yyvsp[-3].sym)->symbol=symbol;
}
#line 2714 "y.tab.c"
    break;

  case 43: /* expression: logic_expression  */
#line 1271 "2005046.y"
{
    fprintf(logout,"expression : logic_expression \n");
    (yyval.sym) = createNode("rule","expression : logic_expression");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2726 "y.tab.c"
    break;

  case 44: /* expression: variable ASSIGNOP logic_expression  */
#line 1279 "2005046.y"
{
    fprintf(logout,"expression : variable ASSIGNOP logic_expression \n");
    (yyval.sym) = createNode("rule","expression : variable ASSIGNOP logic_expression");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2740 "y.tab.c"
    break;

  case 45: /* logic_expression: rel_expression  */
#line 1289 "2005046.y"
{
    fprintf(logout,"logic_expression : rel_expression     \n");
    (yyval.sym) = createNode("rule","logic_expression     : rel_expression");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2752 "y.tab.c"
    break;

  case 46: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 1297 "2005046.y"
{
    fprintf(logout,"logic_expression : rel_expression LOGICOP rel_expression \n");
    (yyval.sym) = createNode("rule","logic_expression : rel_expression LOGICOP rel_expression");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2766 "y.tab.c"
    break;

  case 47: /* rel_expression: simple_expression  */
#line 1307 "2005046.y"
{
    fprintf(logout,"rel_expression  : simple_expression \n");
    (yyval.sym) = createNode("rule","rel_expression  : simple_expression");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2778 "y.tab.c"
    break;

  case 48: /* rel_expression: simple_expression RELOP simple_expression  */
#line 1315 "2005046.y"
{
    fprintf(logout,"rel_expression  : simple_expression RELOP simple_expression \n");
    (yyval.sym) = createNode("rule","rel_expression  : simple_expression RELOP simple_expression");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2792 "y.tab.c"
    break;

  case 49: /* simple_expression: term  */
#line 1326 "2005046.y"
{
    fprintf(logout,"simple_expression : term  \n");
    (yyval.sym) = createNode("rule","simple_expression : term");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2804 "y.tab.c"
    break;

  case 50: /* simple_expression: simple_expression ADDOP term  */
#line 1334 "2005046.y"
{
    fprintf(logout,"simple_expression : simple_expression ADDOP term  \n");
    (yyval.sym) = createNode("rule","simple_expression : simple_expression ADDOP term");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2818 "y.tab.c"
    break;

  case 51: /* term: unary_expression  */
#line 1344 "2005046.y"
{
    fprintf(logout,"term :  unary_expression \n");
    (yyval.sym) = createNode("rule","term :  unary_expression");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2830 "y.tab.c"
    break;

  case 52: /* term: term MULOP unary_expression  */
#line 1352 "2005046.y"
                                   {
    fprintf(logout,"term : term MULOP unary_expression \n");
    (yyval.sym) = createNode("rule","term : term MULOP unary_expression");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());}
#line 2843 "y.tab.c"
    break;

  case 53: /* unary_expression: ADDOP unary_expression  */
#line 1363 "2005046.y"
{   
    fprintf(logout,"unary_expression : ADDOP unary_expression \n");
    (yyval.sym) = createNode("rule","unary_expression : ADDOP unary_expression");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2856 "y.tab.c"
    break;

  case 54: /* unary_expression: NOT unary_expression  */
#line 1372 "2005046.y"
{
    fprintf(logout,"unary_expression : NOT unary_expression \n");
    (yyval.sym) = createNode("rule","unary_expression : NOT unary_expression");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2869 "y.tab.c"
    break;

  case 55: /* unary_expression: factor  */
#line 1381 "2005046.y"
{
    fprintf(logout,"unary_expression : factor \n");
    (yyval.sym) = createNode("rule","unary_expression : factor");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2881 "y.tab.c"
    break;

  case 56: /* factor: variable  */
#line 1389 "2005046.y"
{
    fprintf(logout,"factor  : variable \n");
    (yyval.sym) = createNode("rule","factor  : variable");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2893 "y.tab.c"
    break;

  case 57: /* factor: ID LPAREN argument_list RPAREN  */
#line 1397 "2005046.y"
{
    fprintf(logout,"factor  : ID LPAREN argument_list RPAREN \n");
    (yyval.sym) = createNode("rule","factor  : ID LPAREN argument_list RPAREN");
    addChild((yyval.sym), (yyvsp[-3].sym));
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-3].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2908 "y.tab.c"
    break;

  case 58: /* factor: LPAREN expression RPAREN  */
#line 1408 "2005046.y"
{
    fprintf(logout,"factor : LPAREN expression RPAREN \n");
    (yyval.sym) = createNode("rule","factor : LPAREN expression RPAREN");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2922 "y.tab.c"
    break;

  case 59: /* factor: CONST_INT  */
#line 1418 "2005046.y"
{
    fprintf(logout,"factor : CONST_INT \n");
    (yyval.sym) = createNode("rule","factor : CONST_INT");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

}
#line 2935 "y.tab.c"
    break;

  case 60: /* factor: CONST_FLOAT  */
#line 1427 "2005046.y"
{
    fprintf(logout,"factor : CONST_FLOAT \n");
    (yyval.sym) = createNode("rule","factor : CONST_FLOAT");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

}
#line 2948 "y.tab.c"
    break;

  case 61: /* factor: variable INCOP  */
#line 1436 "2005046.y"
{
    fprintf(logout,"factor : variable INCOP \n");
    (yyval.sym) = createNode("rule","factor : variable INCOP");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());

}
#line 2962 "y.tab.c"
    break;

  case 62: /* factor: variable DECOP  */
#line 1446 "2005046.y"
{
    fprintf(logout,"factor : variable DECOP \n");
    (yyval.sym) = createNode("rule","factor : variable DECOP");
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-1].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2975 "y.tab.c"
    break;

  case 63: /* argument_list: arguments  */
#line 1455 "2005046.y"
{
    fprintf(logout,"argument_list : arguments \n");
    (yyval.sym) = createNode("rule","argument_list : arguments");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 2987 "y.tab.c"
    break;

  case 64: /* argument_list: %empty  */
#line 1462 "2005046.y"
  {
    fprintf(logout,"argument_list : \n");
    (yyval.sym) = createNode("rule","argument_list : ");
}
#line 2996 "y.tab.c"
    break;

  case 65: /* arguments: arguments COMMA logic_expression  */
#line 1468 "2005046.y"
{
    fprintf(logout,"arguments : arguments COMMA logic_expression \n");
    (yyval.sym) = createNode("rule","arguments : arguments COMMA logic_expression");
    addChild((yyval.sym), (yyvsp[-2].sym));
    addChild((yyval.sym), (yyvsp[-1].sym));
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[-2].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
}
#line 3010 "y.tab.c"
    break;

  case 66: /* arguments: logic_expression  */
#line 1478 "2005046.y"
{
    fprintf(logout,"arguments : logic_expression  \n");
    (yyval.sym) = createNode("rule","arguments : logic_expression");
    addChild((yyval.sym), (yyvsp[0].sym));
    (yyval.sym)->setStartLine((yyvsp[0].sym)->getStartLine());
    (yyval.sym)->setEndLine((yyvsp[0].sym)->getEndLine());
	}
#line 3022 "y.tab.c"
    break;


#line 3026 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1485 "2005046.y"


void yyerror(const char *s)
{
    fprintf(stderr,"error: %s\n",s);
}

int main(int argc,char *argv[])
{
    if(argc < 5)
    {
        printf("Usage: %s <input_file> <log_file> <parsetree_file> <assembly_file>\n", argv[0]);
        exit(1);
    }
    if((input=fopen(argv[1],"r"))==NULL)
	{
    printf("Cannot Open Input File.\n");
    exit(1);
	}
    table=new SymbolTable(11);
    logout= fopen(argv[2],"w");
	parseTree=fopen(argv[3],"w");
	assembly=fopen(argv[4],"w");

    yyin=input;
    yyparse();

	fprintf(logout,"Total Lines: %d",line_count);
	fclose(logout);
	fclose(parseTree);
	fclose(assembly);
    fclose(input);
    printf("\nParsing finished\n");
    return 0;
}
