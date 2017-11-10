/*
*
* Ece Naz Sefercioðlu
*
* 150130140
*
*
* */

#include<iostream>
#include <stdlib.h> 
#include <fstream>
#include<stdio.h>
#include<string.h>
#include <sstream>  
#include <vector>
#include <algorithm>
#include<stack>
using namespace std;


class TerminalRule  //class of terminal rules
{
public:
	string  self;  //rule it self
	vector<string> outs; //outputs of the rule
};

class rule {  //right side, output of a rule
public:
	string left, right;
	void setRule(string a, string b) {
		left = a;
		right = b;
	};
	bool operator==(rule a) {   // == asignment overload
		if (this->left == a.left && this->right == a.right)
			return true;
		else return false;
	};
};

class NonTerminalRule { //class of non terminal rules
public:
	string self; //rule it self
	vector<rule> rules; //outputs of the rule
};


vector<TerminalRule> terminals;
vector<NonTerminalRule> nonterminals;
vector<vector<string> > sentences;

TerminalRule Y;
TerminalRule reverseEngOutput(string a) { //given an output word, returns the rule it belongs
	bool end = false;
	int i;
	for ( i = 0; i < terminals.size(); i++)
	{
		for (size_t m = 0; m < terminals[i].outs.size() ; m++)
		{
			if (a == terminals[i].outs[m])
				end = true;
			if (end)break;
		}
		if (end)break;
	}
	if (!end) return Y;
	return terminals[i];
}

NonTerminalRule K;

bool isTerminal(string a) {  //see if the rule belongs to terminal rules
	for (int b = 0; b < terminals.size(); b++) {
		if (terminals[b].self == a)
			return true;
	}
	return false;
}


NonTerminalRule reverseEngTerminal(rule a) {  //given an rule, returns the rule it belongs
	bool end = false;
	int i;
	for ( i = 0; i < nonterminals.size(); i++)
	{
		for (size_t m = 0; m < nonterminals[i].rules.size(); m++)
		{
				if (a == nonterminals[i].rules[m])
					end = true;
				if (end)break;
					//return nonterminals[i];	
		}
		if (end) break;
	}
	if (end)
		return nonterminals[i];
	else
		return K;
}


class Parser
{
public:
int globalTrack; //tracker to see which sentence the program is goingthrough
vector<string> tree;  //parse tree
vector<string> inputSeq; //sentence transformed into language rules

	void extractParseTree() {  //for each sentence extracts parse tree
		for (globalTrack = 0; globalTrack < sentences.size(); globalTrack++)//*sentences.size()
		{
			inputSeqInit(sentences[globalTrack]); //transform sentence into rules
			parse(inputSeq, inputSeq.size() - 1); //parse it
			inputSeq.clear(); //clear variables for next iterations
			tree.clear();
		}
	};

void inputSeqInit(vector<string> sentence) { //reverses the sentence to terminal rules
	for (int k = 0; k <sentence.size(); k++)
	{
		inputSeq.push_back(reverseEngOutput(sentence[k]).self);
	}
};


	void parse(vector<string> sentence, int pointer) { //parses the given sentence, sentence is the unit of rules, pointer is where the windows is
		int i = pointer;
		rule dummy;   //a temp rule to find the maker rule of rules in window
		dummy.setRule(sentence[i - 1], sentence[i]);
		string result = reverseEngTerminal(dummy).self; //if 
	 
		if (result == K.self) { //rule nor found
			pointer--;
			if (pointer != 0) //while window can be shifted, shift and parse again
				parse(sentence, pointer);
			else  //window can not be shifted the sentence does not belong to language
				cout << "SYNTAX ERROR" << endl;
		}
		else if (result != "S") { //rule is not the start rule
			if (tree.size() != 0) { //tree were initiated
				if (dummy.right == sentence[i] && pointer == sentence.size() - 1) {  //there is already a terminal rule, add only the new rule read from sentence
					tree.insert(tree.begin(), "[");
					if (isTerminal(sentence[i - 1]))
						tree.push_back("(" + sentences[globalTrack][i - 1] + ")");
					tree.push_back(sentence[i - 1]);
				}
				else { //new rules to be added to tree
					tree.push_back("[");
					if (isTerminal(sentence[i]))
						tree.push_back("(" + sentences[globalTrack][i] + ")");
					tree.push_back(sentence[i]);
					if (isTerminal(sentence[i - 1]))
						tree.push_back("(" + sentences[globalTrack][i - 1] + ")");
					tree.push_back(sentence[i - 1]);
				}
			}
			else {  //tree was not initiated, first elements
				tree.push_back("[");

				if (isTerminal(sentence[i]))
					tree.push_back("(" + sentences[globalTrack][i] + ")");
				tree.push_back(sentence[i]);

				if (isTerminal(sentence[i - 1]))
					tree.push_back("(" + sentences[globalTrack][i - 1] + ")");
				tree.push_back(sentence[i - 1]);
			}
			sentence.erase(sentence.begin() + i - 1); //clear product rules from sentence 
			sentence.erase(sentence.begin() + i - 1);
			tree.push_back("]"); //end the rule
			if (i <= sentence.size()) {   //add the producer rule
				sentence.insert(sentence.begin() + i - 1, result);
			}
			else {
				sentence.push_back(result);
			}
			tree.push_back(result);
			pointer = sentence.size() - 1;  //update pointer
			parse(sentence, pointer);
		}
		else if (result == "S" && sentence.size()>2) { //If found rule is s but there is more than 2 rules left
			pointer--;
			if (pointer != 0)  //if pointer does not reach the leftmost, shift leftmost
				parse(sentence, pointer);
			else  //if pointer reached the leftmost, word does not belong to language
				cout << "SYNTAX ERROR" << endl;
		} 
		else { //If found rule is s and there is only 2 rules left
			tree.insert(tree.begin(), "[");
			if (dummy.right == tree[tree.size() - 1]) { //if there is a rule left 
				if (isTerminal(sentence[i - 1]))
					tree.push_back("(" + sentences[globalTrack][i - 1] + ")");
				tree.push_back(sentence[i - 1]);
			}
			tree.push_back("]");
			tree.push_back(result); //tree is formed
			treePrint(); //print out the tree
		}
	};


	void treePrint() { //prints out the tree in correct order
		for (int i = tree.size() - 1; i > -1; i--)
		{
			if (tree[i] == "[")
				cout << "]";
			else if (tree[i] == "]")
				cout << "[";
			else
			{
				cout << tree[i];
			}
		}
		cout << endl;
	};
};

void initiateNonTerminals();
void initiateTerminals();
void initSentences(string a); //******


int main(int argc, char *argv[]) {
	string a = argv[1];  //take the input file name







	K.self = "NO"; //error variable for nonterminals
	Y.self = "ON"; //error variable for terminals
	initiateTerminals();   
	initiateNonTerminals();
	initSentences(a);  //read sentences from given file

	Parser groot;
	groot.extractParseTree();
	return 0;
}


void initSentences(string a) //**********
{
	ifstream myfile;             //read sentences from given text file
								
	myfile.open(a.c_str());  //***********************
//	myfile.open("input.txt");
	string num;
	int noOfWord;
	vector<string> oneSentence;
	if (myfile.is_open())
	{
		while (getline(myfile, num))  //reads each line and constructructs a sentence vector and adds it to the sentences vector
		{
			num[0] = tolower(num[0]);
			istringstream iss(num);
			noOfWord = count(num.begin(), num.end(), ' ') + 1;
			string temp;			
			for (size_t i = 0; i < noOfWord; i++)
			{
				iss >> temp;
				oneSentence.push_back(temp);
			}
			sentences.push_back(oneSentence);
			oneSentence.clear();
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	myfile.close();
}

void initiateNonTerminals() {
	NonTerminalRule dumme;  //Initializes nonterminal rules of the language
	rule  ter;
	dumme.self = "NP";
	ter.setRule("NN", "NN");
	dumme.rules.push_back(ter);
	ter.setRule("AD", "NN");
	dumme.rules.push_back(ter);
	ter.setRule("AD", "NP");
	dumme.rules.push_back(ter);
	ter.setRule("DT", "NP");
	dumme.rules.push_back(ter);
	ter.setRule("DT", "NN");
	dumme.rules.push_back(ter);
	nonterminals.push_back(dumme);
	dumme.rules.clear();
	
	dumme.self = "PP";
	ter.setRule("PR", "NN");
	dumme.rules.push_back(ter);
	ter.setRule("PR", "NP");
	dumme.rules.push_back(ter);
	nonterminals.push_back(dumme);
	dumme.rules.clear();
	
	dumme.self = "VP";
	ter.setRule("VR", "PP");
	dumme.rules.push_back(ter);
	ter.setRule("NN", "VR");
	dumme.rules.push_back(ter);
	ter.setRule("NP", "VR");
	dumme.rules.push_back(ter);
	ter.setRule("VR", "NP");
	dumme.rules.push_back(ter);
	nonterminals.push_back(dumme);
	dumme.rules.clear();
	
	dumme.self = "S";
	ter.setRule("NP", "VP");
	dumme.rules.push_back(ter);
	ter.setRule("NN", "VP");
	dumme.rules.push_back(ter);
	nonterminals.push_back(dumme);
	dumme.rules.clear();
}


void initiateTerminals() {	 //Initializes nonterminal rules of the language 
	TerminalRule dummy;
	dummy.self = "DT";
	dummy.outs.push_back("that");
	dummy.outs.push_back("this");
	dummy.outs.push_back("a");
	dummy.outs.push_back("the");
	terminals.push_back(dummy);
	
	dummy.self = "NN";
	dummy.outs.clear();
	dummy.outs.push_back("book");
	dummy.outs.push_back("flight");
	dummy.outs.push_back("cat");
	dummy.outs.push_back("mat");
	dummy.outs.push_back("i");
	dummy.outs.push_back("you");
	dummy.outs.push_back("they");
	terminals.push_back(dummy);
	
	dummy.self = "VR";
	dummy.outs.clear();
	dummy.outs.push_back("booked");
	dummy.outs.push_back("included");
	dummy.outs.push_back("preferred");
	dummy.outs.push_back("sat");
	terminals.push_back(dummy);
	
	dummy.self = "PR";
	dummy.outs.clear();
	dummy.outs.push_back("from");
	dummy.outs.push_back("to");
	dummy.outs.push_back("on");
	dummy.outs.push_back("near");
	dummy.outs.push_back("through");
	terminals.push_back(dummy);
	
	dummy.self = "AD";
	dummy.outs.clear();
	dummy.outs.push_back("big");
	dummy.outs.push_back("heavy");
	dummy.outs.push_back("beautiful");
	dummy.outs.push_back("cheap");
	terminals.push_back(dummy);
}