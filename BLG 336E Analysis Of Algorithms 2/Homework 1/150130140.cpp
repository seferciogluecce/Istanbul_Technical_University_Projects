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
#include <sstream>  //isstream
#include <vector>
#include<list>
#include<stack>
using namespace std;

class Block //block class
{
public:
	Block(int a, int b, int d, char c) 
	{ x = a; y = b; direction = c; length = d; };
	Block() {};
	void setX(int a) { x = a; };
	void setY(int a) { y = a; };
	void setDirection(char a) { direction = a; };
	void setLength(int a) { length = a; };
	int getLength() { return length; };
	int getX() { return x; };
	int getY() { return y; };
	char getDirection() { return direction; };

private:
	int x;
	int y;
	int length;
	char direction;
};


class State //state class
{ 
public:
	vector<State> parents;
	vector<Block> blocks;
	int id;
	vector<vector<int > > fullMat;

	void matFill();
	void printState();
	bool isWon();
	State moveLeft(int order);
	State moveUp(int order);
	State moveRight(int order);
	State moveDown(int order);
	bool canLeft(int order);
	bool canRight(int order);
	bool canUp(int order);
	bool canDown(int order);
	bool noPath(State);
	State() {
	};
	State(int a) {
		id = a;
	};

	State operator=(State a) {   // = asignment overload
		this->parents = a.parents;
		this->blocks = a.blocks;
		this->fullMat = a.fullMat;
		this->id = a.id;
		return *this;
	};
private:

};
bool State::noPath(State x) { //checks if the state is unique
								//cycle check for dfs
	for (int i = 0; i < parents.size(); i++)
	{
		if (parents[i].fullMat == x.fullMat) {
			return false;
		}
	}
	return true;
}

bool noPath(vector<State>y, State x) {  //cycle check for bfs
	for (int i = 0; i < y.size(); i++)
	{
		if (y[i].fullMat == x.fullMat) {
			return false;
		}
	}
	return true;
}


bool State::canLeft(int order) { //checks if the move is possible for given block
	if (blocks[order].getDirection() == 'h')
		return false;
	if (blocks[order].getX() - blocks[order].getLength() + 1 != 0 && (fullMat[blocks[order].getX() - blocks[order].getLength()][blocks[order].getY()]) == 0)
	{
		return true;
	}
	else
		return false;
}
bool State::canRight(int order) {
	if (blocks[order].getDirection() == 'h')
		return false;
	if (blocks[order].getX() != 5 && (fullMat[blocks[order].getX() + 1][blocks[order].getY()]) == 0)
	{
		return true;
	}
	else
		return false;
}
bool State::canUp(int order) {
	if (blocks[order].getDirection() == 'v')
		return false;
	if (blocks[order].getY() + blocks[order].getLength() - 1 != 5 && (fullMat[blocks[order].getX()][blocks[order].getY() + blocks[order].getLength()]) == 0)
	{
		return true;
	}
	else
		return false;
}
bool State::canDown(int order) {
	if (blocks[order].getDirection() == 'v')
		return false;
	if (blocks[order].getY() != 0 && (fullMat[blocks[order].getX()][blocks[order].getY() - 1]) == 0)
	{
		return true;
	}
	else
		return false;
}
State State::moveLeft(int order) { //moves 1 block the given ordered block in specified direction
	State temp = *this;
	int label = temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY()];
	temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY()] = 0; //clean left space
	temp.fullMat[temp.blocks[order].getX() - temp.blocks[order].getLength()][temp.blocks[order].getY()] = label; //fill new space 
	temp.blocks[order].setX(temp.blocks[order].getX() - 1);
	return temp;
}

State State::moveRight(int order) {
	State temp = *this;
	int label = temp.fullMat[temp.blocks[order].getX() - temp.blocks[order].getLength() + 1][temp.blocks[order].getY()];
	temp.fullMat[temp.blocks[order].getX() - temp.blocks[order].getLength() + 1][temp.blocks[order].getY()] = 0;
	temp.fullMat[temp.blocks[order].getX() + 1][temp.blocks[order].getY()] = label;
	temp.blocks[order].setX(temp.blocks[order].getX() + 1);
	return temp;
}
State State::moveUp(int order) {
	State temp = *this;
	int label = temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY()];
	temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY()] = 0;
	temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY() + temp.blocks[order].getLength()] = label;
	temp.blocks[order].setY(temp.blocks[order].getY() + 1);
	return temp;
}
State State::moveDown(int order) {
	State temp = *this;
	int label = temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY() + temp.blocks[order].getLength() - 1];
	temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY() + temp.blocks[order].getLength() - 1] = 0;
	temp.fullMat[temp.blocks[order].getX()][temp.blocks[order].getY() - 1] = label;
	temp.blocks[order].setY(temp.blocks[order].getY() - 1);
	return temp;
}

void State::matFill() { //visualizes the mat with respected order of the blocks
	for (int a = 0; a < blocks.size(); a++) {
		int t;
		for (int b = 0; b < blocks[a].getLength(); b++) {
			if (a == 0) { t = 9; }
			else t = a;
			if (blocks[a].getDirection() == 'v') {
				fullMat[blocks[a].getX() - b][blocks[a].getY()] = t;
			}
			else
			{
				fullMat[blocks[a].getX()][blocks[a].getY() + b] = t;
			}
		}
	}
}

bool State::isWon() {  //checks win condition
	bool yep = true;
	for (int a = blocks[0].getY() + blocks[0].getLength(); a < 6; a++) {
		if (fullMat[blocks[0].getX()][a] != 0) {
			yep = false;
			break;
		}
	}
	return yep;
}

void State::printState() {  //prints out the mat of the state on console
	cout << endl;
	for (int mx = 0; mx < 6; mx++) {
		cout << " | ";
		for (int my = 0; my < 6; my++) {
			cout << fullMat[mx][my] << " | ";
		}
		cout << endl;
	}
}

class edge { //edge class
public:
	int u;
	int v;
	edge() {};
	edge(int a, int b) { u = a; v = b; };
};

int maxMoveCalc(vector<Block> x) { //calculates the total number of moves
	int max = 1;
	for (int i = 0; i < x.size(); i++)
	{
		max = max*(6 - x[i].getLength());
	}
	return max;
}





void printStatePath(vector<State> arr);
void printStatePathDFS(vector<edge> arr, vector<State> arr2);
void printStatePathBFS(vector<edge> arr, vector<State> arr2);
void printToFile(string fileName, vector<State> arr);
void printToFileDFS(string fileName, vector<edge> arr, vector<State> arr2);




int main(int argc, char *agv[]) {
	
	string traverseType = agv[1];
	string inFileName = agv[2];
	string outFileName = agv[3];
	
//	string traverseType;
	//cin >> traverseType;
	vector <int> dummy;
	State init;
	for (int mx = 0; mx < 6; mx++) {  //fill the mat with full zeros
		init.fullMat.push_back(dummy);
		for (int my = 0; my < 6; my++) {
			init.fullMat[mx].push_back(0);
		}
	}
	ifstream myfile;             //read blocks from blocks.txt
	//myfile.open("blocks.txt");
	myfile.open(inFileName.c_str());
	string num;
	int px, py, pl;
	char pd;
	if (myfile.is_open())
	{
		int i = 0;
		while (getline(myfile, num))  //reads each line and constructracts a block object and push it to first states blocks
		{
			istringstream iss(num);
			string temp;

			iss >> temp;
			px = atoi(temp.c_str());

			iss >> temp;
			py = atoi(temp.c_str());

			iss >> temp;
			pl = atoi(temp.c_str());

			iss >> temp;
			pd = *temp.c_str();

			Block temper(px - 1, py - 1, pl, pd);

			init.blocks.push_back(temper);

			i++;
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	init.matFill(); //fill the first mat in accordance to blocks

	int maxMove = maxMoveCalc(init.blocks); //calculation of possible nmber of moves

	init.id = 0;
	bool won = false;

	if (traverseType=="bfs") {  //bfs traverse         
		vector<State> royals;  //unique states are saved here
		vector <vector <State> > L;
		bool * discovered = new bool[maxMove];
		vector<edge> BFS;
		vector<State>dummy; //dummy state to initiate L
		dummy.push_back(init);
		int idCounter = 1; //id counter for states
		discovered[0] = true;
		int iy = 0;
		vector<State> dummy2; //dummy state to initiate l i+1
		L.push_back(dummy);//L i
		while (L[iy].size() != 0 && !won) {
			L.push_back(dummy2);//L i+1
			for (int a = 0; a < L[iy].size() && !won; a++) 
			{ //for each node in l i
				for (int i = 0; i < L[iy][a].blocks.size(); i++) //find each connected edge
				{
					if (L[iy][a].canUp(i)) {   //moveability check
						if (noPath(royals, L[iy][a].moveUp(i))) { //if the moved state exists in spce check
							L[iy + 1].push_back(L[iy][a].moveUp(i));	//add state to next layer
							L[iy + 1][L[iy + 1].size() - 1].parents = L[iy][a].parents; //update parents(path) of the state
							L[iy + 1][L[iy + 1].size() - 1].parents.push_back(L[iy][a]);  
							L[iy + 1][L[iy + 1].size() - 1].id = idCounter++;  //set id of the state
							discovered[L[iy + 1][L[iy + 1].size() - 1].id] = true;  //set discovered true
							royals.push_back(L[iy + 1][L[iy + 1].size() - 1]); //add it to unique state vector

							edge temp(L[iy][a].id, L[iy + 1][L[iy + 1].size() - 1].id); //add edge to bfs
							BFS.push_back(temp);

							if (L[iy + 1][L[iy + 1].size() - 1].isWon()) { won = true; break; } //win condition check
						}
					}
					if (L[iy][a].canDown(i)) {
						if (noPath(royals, L[iy][a].moveDown(i))) {
							L[iy + 1].push_back(L[iy][a].moveDown(i));
							L[iy + 1][L[iy + 1].size() - 1].parents = L[iy][a].parents;
							L[iy + 1][L[iy + 1].size() - 1].parents.push_back(L[iy][a]);
							L[iy + 1][L[iy + 1].size() - 1].id = idCounter++;
							discovered[L[iy + 1][L[iy + 1].size() - 1].id] = true;
							royals.push_back(L[iy + 1][L[iy + 1].size() - 1]);
							edge temp(L[iy][a].id, L[iy + 1][L[iy + 1].size() - 1].id);
							BFS.push_back(temp);
							if (L[iy + 1][L[iy + 1].size() - 1].isWon()) { won = true; break; }
						}
					}
					if (L[iy][a].canLeft(i)) {
						if (noPath(royals, L[iy][a].moveLeft(i))) {
							L[iy + 1].push_back(L[iy][a].moveLeft(i));
							L[iy + 1][L[iy + 1].size() - 1].parents = L[iy][a].parents;
							L[iy + 1][L[iy + 1].size() - 1].parents.push_back(L[iy][a]);
							L[iy + 1][L[iy + 1].size() - 1].id = idCounter++;
							discovered[L[iy + 1][L[iy + 1].size() - 1].id] = true;
							royals.push_back(L[iy + 1][L[iy + 1].size() - 1]);
							edge temp(L[iy][a].id, L[iy + 1][L[iy + 1].size() - 1].id);
							BFS.push_back(temp);
							if (L[iy + 1][L[iy + 1].size() - 1].isWon()) { won = true; break; }
						}
					}
					if (L[iy][a].canRight(i)) {
						if (noPath(royals, L[iy][a].moveRight(i))) {
							L[iy + 1].push_back(L[iy][a].moveRight(i));
							L[iy + 1][L[iy + 1].size() - 1].parents = L[iy][a].parents;
							L[iy + 1][L[iy + 1].size() - 1].parents.push_back(L[iy][a]);
							L[iy + 1][L[iy + 1].size() - 1].id = idCounter++;
							discovered[L[iy + 1][L[iy + 1].size() - 1].id] = true;
							royals.push_back(L[iy + 1][L[iy + 1].size() - 1]);
							edge temp(L[iy][a].id, L[iy + 1][L[iy + 1].size() - 1].id);
							BFS.push_back(temp);
							if (L[iy + 1][L[iy + 1].size() - 1].isWon()) { won = true; break; }
						}
					}
				}
			}
			iy++;
		}

		royals[BFS.size() - 1].parents.push_back(royals[BFS.size() - 1]); //add last state to path
	//	printStatePath(royals[BFS.size() - 1].parents);//prints states to console
		printToFile(outFileName,royals[BFS.size() - 1].parents);  //print path to file
		delete[] discovered;
	}
	else {   //dfs traverse

	vector<edge>dfs;
	stack<State> waitSide; //main stact
	State temp;//dummy state to initiate init's parent id
	temp.id = 0;
	init.parents.push_back(temp);
	waitSide.push(init);
	State dontWait; //state to save popped states from the main stack
	vector<State> royalFamily; //state to save unique states

	bool * explored = new bool[maxMove];
	for (int i = 0; i < maxMove; i++) {
		explored[i] = false;
	}
	won = false;
	int IdCounter = 1;

	while (waitSide.size() != 0 && !won) {
		dontWait = waitSide.top(); //pop th e top
		waitSide.pop();
		if (explored[dontWait.id] == false) {   //check if explored
			explored[dontWait.id] = true;
			if (noPath(royalFamily, dontWait)) { // check if it is in the unique states
				edge temp(dontWait.parents[0].id, dontWait.id); //add its and its parents id to dfs tree
				dfs.push_back(temp);
				for (int i = 0; i < dontWait.blocks.size(); i++) //find each connected edge
				{
					if (dontWait.canUp(i)) { 
						if (noPath(royalFamily, dontWait.moveUp(i))) {
							waitSide.push(dontWait);   //add parent of the new state to stack
							waitSide.push(dontWait.moveUp(i)); //add new state to stack
							waitSide.top().parents[0] = dontWait; //give parent to new state
							waitSide.top().id = IdCounter++;
							royalFamily.push_back(dontWait); //add new state to unique states
							if (waitSide.top().isWon()) { won = true; break; } //check win condition
						}
					}
					if (dontWait.canDown(i)) {
						if (noPath(royalFamily, dontWait.moveDown(i))) {
							waitSide.push(dontWait);
							waitSide.push(dontWait.moveDown(i));
							waitSide.top().parents[0] = dontWait;
							waitSide.top().id = IdCounter++;
							royalFamily.push_back(dontWait);
							if (waitSide.top().isWon()) { won = true; break; }
						}
					}if (dontWait.canLeft(i)) {
						if (noPath(royalFamily, dontWait.moveLeft(i))) {
							waitSide.push(dontWait);
							waitSide.push(dontWait.moveLeft(i));
							waitSide.top().parents[0] = dontWait;
							waitSide.top().id = IdCounter++;
							royalFamily.push_back(dontWait);
							if (waitSide.top().isWon()) { won = true; break; }
						}
					}if (dontWait.canRight(i)) {
						if (noPath(royalFamily, dontWait.moveRight(i))) {
							waitSide.push(dontWait);
							waitSide.push(dontWait.moveRight(i));
							waitSide.top().parents[0] = dontWait;
							waitSide.top().id = IdCounter++;
							royalFamily.push_back(dontWait);
							if (waitSide.top().isWon()) { won = true; break; }
						}
					}

				}
			}
		}
	}

	edge x(waitSide.top().id, waitSide.top().id); //make an edge for last state and add it to bfs
	dfs.push_back(x);
	royalFamily.push_back(waitSide.top()); //add last state to unique states vector

	//printStatePathDFS(dfs, royalFamily);//prints statets to console
	printToFileDFS(outFileName, dfs, royalFamily); //print to file 
	delete[] explored;
}

	return 0;
}


void printPath(vector<State> arr) {  //prints states of the path on console as its requested in txt format
	for (int s = 0; s < arr.size(); s++) {
		for (int sa = 0; sa < arr[s].blocks.size(); sa++) {
			cout << arr[s].blocks[sa].getX() + 1 << " " << arr[s].blocks[sa].getY() + 1 << " " << arr[s].blocks[sa].getLength() << " " << arr[s].blocks[sa].getDirection() << endl;
		}
	}
}

void printStatePath(vector<State> arr) { //prints mats of the states of given vector on console
	for (int s = 0; s < arr.size(); s++) {
		arr[s].printState();
	}
}

void printToFile(string fileName, vector<State> arr) {//prints states of the path on file
	ofstream myoutput;
	myoutput.open(fileName.c_str());
	for (int s = 0; s < arr.size(); s++) {
		for (int sa = 0; sa < arr[s].blocks.size(); sa++) {
			myoutput << arr[s].blocks[sa].getX() + 1 << " " << arr[s].blocks[sa].getY() + 1 << " " << arr[s].blocks[sa].getLength() << " " << arr[s].blocks[sa].getDirection() << endl;
		}
		myoutput << endl;
	}
	myoutput.close();
}
void printToFileDFS(string fileName, vector<edge> arr, vector<State> arr2) { //prints states of the path on file
	vector<State> print;
	for (int i = 0; i < arr.size(); i++)
	{
		print.push_back(arr2[arr[i].v]);
	}
	ofstream myoutput;
	myoutput.open(fileName.c_str());
	for (int s = 0; s < print.size(); s++) {
		for (int sa = 0; sa < print[s].blocks.size(); sa++) {
			myoutput << print[s].blocks[sa].getX() + 1 << " " << print[s].blocks[sa].getY() + 1 << " " << print[s].blocks[sa].getLength() << " " << print[s].blocks[sa].getDirection() << endl;
		}
		myoutput << endl;
	}
}
void printStatePathBFS(vector<edge> arr, vector<State> arr2) { //prints states of the path on console
	int last;
	last = arr.size() - 1;
	for (size_t i = arr.size() - 1; i >= 0; i--)
	{
		if (arr[last].u == arr[i].v) {
			arr2[i].printState();
		}
	}
}
void printStatePathDFS(vector<edge> arr, vector<State> arr2) { //prints states of the path on console
	vector<State> print;
	for (int i = 0; i < arr.size(); i++)
	{
		arr2[arr[i].v].printState();
		//print.push_back(arr2[arr[i].v]);
	}
	//printToFile("outdfs.txt", print);
}
