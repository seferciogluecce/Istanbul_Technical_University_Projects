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
#include <string>     
#include <string.h>     
#include <cstring>     
#include<queue>
#include<algorithm>
#include<climits>

using namespace std;

class Publication   
{
public:
	int revCount;  //how much review it needs
	string type;   //type of the publication
	int order;		//order of the given publication
	vector<string> reviewer;
	Publication() {};
	Publication(string a, int b) {
		type = a;
		order = b;
		if (a == "Journal")
		{
			revCount = 2;
		}
		else if (a == "WorkProc")
		{
			revCount = 2;
		}
		else {
			revCount = 3;
		}
	};

	~Publication() {};
};

class Reviewer
{
public:
	string title;   //title of the reviewer
	int time;		//max number of review they have
	int order;		//order of the reviewer
	Reviewer() {};
	Reviewer(string a, int b, int c)
	{
		title = a;
		time = c;
		order = b;
	}
	~Reviewer() {};
};


void initPublication(string a);  //reads from file and constructs pubs vector
void initReviewer(string a);  //reads from file and constructs revs vector
vector<Reviewer> revs;  //vector contains reviewers
vector<Publication> pubs; //vector contains publications
int **graph;  //main graph
int **resGraph; //residual graph
void graphfy3(); //takes publications and reviewers and tranfsforms the data to adjacency matrix
void ffa(); ///ford fulkerson algorithm implementation regard to assignment
bool bfs(int *); // bread first search to find a path from source to sink
int sizeOfGraph; //size of the graph #revs+#pubs+ s+ t
void outPut(string a); //output function
void resGraphy(); //copies graph to recidual graph

int main(int argc, char *argv[]) {
		initReviewer(argv[1]);   //initialize te objects
		initPublication(argv[2]);

//	initPublication("publications.txt");
	//initReviewer("reviewer2.txt");

	graphfy3();  //make the grap of problem

	sizeOfGraph = revs.size() + 2 + pubs.size();

	resGraphy(); //copy graph to residual graph

	ffa(); //implement ford fulkerson algorithm to data

	outPut("output.txt");

	return 0;
}

void resGraphy() { //copies graph to residual graph
	resGraph = new int*[sizeOfGraph];
	
	for (int a = 0; a < sizeOfGraph; a++)
		resGraph[a] = new int[sizeOfGraph];
	
	for (int a = 0; a < sizeOfGraph; a++)
		for (int b = 0; b < sizeOfGraph; b++)
			resGraph[a][b] = graph[a][b];
}

void outPut(string a) {  //prints out the matches and non matches if any
	ofstream myoutput;
	myoutput.open(a.c_str());

	for (int a = 0; a < pubs.size(); a++) {

		if (pubs[a].reviewer.size() !=0) {

			myoutput << pubs[a].type << pubs[a].order << " is assigned to ";
			for (int i = 0; i < pubs[a].reviewer.size(); i++)
			{
				myoutput << pubs[a].reviewer[i];

				if (i != pubs[a].reviewer.size() - 1)
					myoutput << ", ";
			}
			myoutput << endl;
		}
		else {
			myoutput << pubs[a].type << pubs[a].order << " is NOT assigned." << endl;
		}
	}
	myoutput.close();
}


bool bfs(int *parent) {  //searches for a path from s to t and stores the source of destination to parent

	bool *discovered = new bool[sizeOfGraph];  

	int a = 0;
	while (a != sizeOfGraph) {
		discovered[a] = false;
		a++;
	}

	queue <int> bfs;
	bfs.push(0); //put s to bfs queue
	parent[0] = -1; //parent is < 0 for s
	discovered[0] = true;

	while (bfs.size()!=0) { 
		int current = bfs.front();
		bfs.pop();
		for (int i = 0; i < sizeOfGraph; i++) {

			if (discovered[i] == false && resGraph[current][i] > 0) {  //when we find connected and undiscovered node s we add them to queue to inspect later
				bfs.push(i);
				parent[i] = current;  // node i and current are connected, current is the parent
				discovered[i] = true;
			}
		}
	}

	if (discovered[sizeOfGraph - 1] == true) {  //t is reached we found a path from s to t
		return true;
	}
	else {    //t could not be reached
		return false;
	}

}

void ffa() { ///ford fulkerson algorithm implemantation regard to assignment


	int * path = new int[sizeOfGraph];  //path tp path from each node
	int left; //left side of the 2d graph

	while (bfs(path)) {


		int updateFlow = INT_MAX;  // we want to minimize it 
		int s = sizeOfGraph - 1;  //start from t and trace back to s+1
		int pub; //indice of publication to store
		while (s != 0) { //from the to s+1 we calculate update flow and matches to publications


			if (s > 0 && s < revs.size() + 1) { //margin for reviewers
				stringstream x;
				x << revs[s - 1].order;
				pubs[pub].reviewer.push_back(revs[s - 1].title + x.str());
			}
			else if (s > revs.size() && s < sizeOfGraph - 1) { //margin for publications
				pub = s - revs.size() - 1;
			}

			left = path[s]; 

			if (updateFlow > resGraph[left][s])  //updates the current flow
			{
				updateFlow = resGraph[left][s];
			}

			s = left;
		}

		int d;
		s = sizeOfGraph - 1;

		while (s != 0) {  //updates the value of residual graph

			d = path[s];
			resGraph[d][s] -= updateFlow;
			resGraph[s][d] += updateFlow;

			s = d;
		}

		delete[]path;
		path = new int[sizeOfGraph];

	}

}



void graphfy3() {  //constructs the graph in s - reviewers - publiacations - t order

	graph = new int*[revs.size() + 2 + pubs.size()];

	for (int j = 0; j < revs.size() + 2 + pubs.size(); j++)
	{
		graph[j] = new int[revs.size() + 2 + pubs.size()];
		for (int i = 0; i < revs.size() + 2 + pubs.size(); i++)
		{

			if (j == 0 && i > 0 && i < revs.size() + 1) {
				graph[j][i] = revs[i - 1].time;
			}
			else if (j > 0 && j<revs.size() + 1 && i>revs.size() && i < revs.size() + pubs.size() + 1)
			{
				if (revs[j - 1].title == "Prof") {
					graph[j][i] = 1;
				}
				else if (revs[j - 1].title == "AstProf" && pubs[i - revs.size() - 1].type != "Journal")
				{
					graph[j][i] = 1;

				}
				else if (revs[j - 1].title == "Ast" && pubs[i - revs.size() - 1].type != "Journal" && pubs[i - revs.size() - 1].type != "ConfProc")
				{
					graph[j][i] = 1;

				}
				else {
					graph[j][i] = 0;

				}

			}
			else if (i > revs.size() + pubs.size() && j > revs.size() && j < revs.size() + pubs.size() + 1) {
				graph[j][i] = pubs[j - revs.size() - 1].revCount;

			}
			else {
				graph[j][i] = 0;

			}

		}

	}
}



void initReviewer(string a) {

	ifstream myfile;             //read sentences from given text file

	myfile.open(a.c_str());  //***********************
	string num;
	string name;
	string order;
	string counts;
	string type;
	if (myfile.is_open())
	{
		getline(myfile, num);

		while (getline(myfile, num))  //reads each reviewer and constructructs a reviewer object and adds it to the revs vector
		{

			istringstream iss(num);
			iss >> name;

			iss >> counts;

			if (name[3] == 'f') {//prof
				order = name.substr(4);
				type = "Prof";
			}
			else if (name[3] == 'P') {//astprof
				order = name.substr(7);

				type = "AstProf";
			}
			else { //ast
				order = name.substr(3);
				type = "Ast";
			}
			cout << name << " " << atoi(order.c_str()) << " " << atoi(counts.c_str()) << endl;
			Reviewer temp(type, atoi(order.c_str()), atoi(counts.c_str()));


			revs.push_back(temp);

		}
		myfile.close();
	}
	else cout << "Unable to open file";
	myfile.close();

}

void initPublication(string a) {
	ifstream myfile;             //read publications from given text file

	myfile.open(a.c_str());  //***********************
	string num;
	string name;
	string type;
	string order;
	if (myfile.is_open())
	{
		getline(myfile, num);

		while (getline(myfile, num))  //reads each line and constructructs a publication object and adds it to the pubs vector
		{
			istringstream iss(num);
			iss >> name;


			if (name[0] == 'J') {//journal
				order = name.substr(7);
				type = "Journal";
			}
			else if (name[0] == 'C') {//confproc
				order = name.substr(8);

				type = "ConfProc";
			}
			else { //workproc
				order = name.substr(8);
				type = "WorkProc";
			}
			cout << type << " " << atoi(order.c_str()) << endl;
			Publication temp(type, atoi(order.c_str()));
			pubs.push_back(temp);
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	myfile.close();

}











