/*
*
* Ece Naz Sefercioğlu
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
#include <math.h> //sqrt
#include<time.h>


using namespace std;



class Player {   // Object for a Player
	string id;    //members of the object
	int CP;
	string type;
public:
	void setId(string a) { id = a; };   //get-set functions for members of the object
	string getId() { return id; };

	void setType(string a) { type = a; };
	string getType() { return type; };

	void setCP(int a) { CP = a; };
	int getCP() { return CP; };

	Player operator=(Player a) {   // = asignment overload
		this->setId(a.getId());
		this->setCP(a.getCP());
		this->setType(a.getType());
		return *this;
	}
};

void HEAPSORT(Player a[]);
void BUILD_MAX_HEAP(Player a[]);
void MAX_HEAPIFY(Player a[], int i);
int heapsize;  //global variables for heapsize and lengthA
int lengthA;



int main(int argc,char *agv[])
{
	clock_t c; // initiation for calculation of execution time 
	c=clock();
	
	
	int Part=atoi(agv[1]); // choose which part to execute
	
	
	if(Part==1){   // first part of the program
	  
	 int toSort= atoi(agv[2]);  //number of inputs to sort
	 lengthA = toSort;
      
	Player* sortA = new Player[toSort+1];  // array initiation for players to sort

	ifstream myfile;             //read from ClanA.txt
	myfile.open("ClanA.txt");
	string num;
	if (myfile.is_open())
	{
		unsigned int i = 1;
		while (getline(myfile, num) && i<=toSort)  //reads each line and assigns it to a player inside array
		{

			istringstream iss(num);
			string temp;

			iss >> temp;                   //id of the player
			sortA[i].setId(temp.c_str());


			iss >> temp;                  //CP of the player
			sortA[i].setCP(atoi(temp.c_str()));
			
			
			sortA[i].setType("Soldier");  //type of the player


			i++;
		}

		myfile.close();
	}

	else cout << "Unable to open file";


	HEAPSORT(sortA);   // sort the array using heapsort

	ofstream myoutput;     // opens a new A_sorted.txt file and writes the sorted players in order
	myoutput.open("A_sorted.txt");

	for (unsigned int t = 1; t<=toSort; t++) {
		myoutput << sortA[t].getId() << "\t" << sortA[t].getCP() << endl;
	}
	
	
	delete [] sortA;  //memory allocation

	
	c=clock()-c;

	cout<< " Execution time " << ((float)c)/CLOCKS_PER_SEC << " seconds."<< endl;  // print the execution time


}
else if (Part==2){  //second part of the program
  
	int toSort= 10000;  // we take first 10000 players from each clan
	
	lengthA = toSort;

	
	Player* sortA = new Player[toSort+1];  // array initiation for clan a players to sort

	ifstream myfile;             //read from ClanA.txt
	myfile.open("ClanA.txt");
	string num;
	if (myfile.is_open())
	{
		unsigned int i = 1;
		while (getline(myfile, num) && i<=toSort)  //reads each line and assigns it to a player inside array
		{

			istringstream iss(num);
			string temp;

			iss >> temp;                   //assigns id
			sortA[i].setId(temp.c_str());


			iss >> temp;                   // assigns CP
			sortA[i].setCP(atoi(temp.c_str()));
			
			
			sortA[i].setType("Soldier");   //assigns type
			
			
			i++;
		}

		myfile.close();
	}
	else cout << "Unable to open  ClanA file";

	HEAPSORT(sortA);  //sort the players inside clan A
	
	
	
	Player* sortB = new Player[toSort+1];  // array initiation for clan B players to sort

	myfile.open("ClanB.txt");
	if (myfile.is_open())
	{
		unsigned int i = 1;
		while (getline(myfile, num) && i<=toSort)  //reads each line and assigns it to a player inside array
		{

			istringstream iss(num);
			string temp;

			iss >> temp;                  //assigns id
			sortB[i].setId(temp.c_str());


			iss >> temp;		      // assigns CP
			sortB[i].setCP(atoi(temp.c_str()));


			sortB[i].setType("Soldier");  // assigns type


			i++;
		}

		myfile.close();
	}

	else cout << "Unable to open B file";
	
	
	HEAPSORT(sortB); //sort players inside clan B

	
	sortA[toSort].setType("Leader");   // assign the types of players with respect to their places on heap tree
	for(int i=1;i<8;i++)			//for clan A
	sortA[toSort - i].setType("Henchmen");
	
	sortB[toSort].setType("Leader");		// for clan B
	for (int i = 1; i<8; i++)
	sortB[toSort - i].setType("Henchmen");

	
	
	
	myfile.open("gamelogs.txt");  // read succesful attacks from gamelogs.txt
	string attackerC;  //clan of the attacker
	int attackerPos;   //position inside the sorted array of attacker
	int attackedPos;   //position inside the sorted array of attacked
	int attackerLvl;   //level of attacker inside heap tree
	int attackedLvl;   //level of attacked inside heap tree
	int height;        //level difference between attacker and attacked
	
	if (myfile.is_open())
	{
		while ( getline(myfile, num))  //reads each line and assigns it to a number inside array
		{

			istringstream iss(num);
			string temp;
			iss >> temp;               //attack no
			
			iss >> temp;
			attackerC = temp.c_str();  //clan of attacker
			
			iss >> temp;
			attackerPos = toSort  - atoi(temp.c_str());       //position inside the sorted array of attacker
			attackerLvl=floor(log2(atoi(temp.c_str())+1));    //level of attacker inside heap tree
			
			
			iss >> temp;
			attackedPos = toSort  - atoi(temp.c_str());      //position inside the sorted array of attacked
			attackedLvl=floor(log2(atoi(temp.c_str())+1));   //level of attacked inside heap tree
			
			height = abs(attackerLvl-attackedLvl);           //level difference between attacker and attacked
	
			
			if (attackerC == "A") {  // attacker belongs to clan A
					if (sortA[attackerPos].getType() == "Leader") {  //Attacker is leader
						sortA[attackerPos].setCP(sortA[attackerPos].getCP() + sortB[attackedPos].getCP() / 2);
						sortB[attackedPos].setCP(sortB[attackedPos].getCP() - sortB[attackedPos].getCP() / 2);
					}
					else if (sortA[attackerPos].getType() == "Henchmen") {  //attacker is henchmen
						sortA[attackerPos].setCP(sortA[attackerPos].getCP() + 500);
						if (sortB[attackedPos].getCP() < 500) {
						      sortB[attackedPos].setCP(0);
						}
						else
						sortB[attackedPos].setCP(sortB[attackedPos].getCP() - 500);
					}
					else if (sortA[attackerPos].getType() == "Soldier") {  //attacker is soldier
						sortA[attackerPos].setCP((height+ 1) * 30 + sortA[attackerPos].getCP());
						if (sortB[attackedPos].getCP() < 120) {
							sortB[attackedPos].setCP(0);
						}
						else
						sortB[attackedPos].setCP(sortB[attackedPos].getCP() - 120);
					}
			}
			else{ // attacker belongs to clan B
					if (sortB[attackerPos].getType() == "Leader") {  //Attacker is leader
						sortB[attackerPos].setCP(sortB[attackerPos].getCP() + sortA[attackedPos].getCP() / 2);
						sortA[attackedPos].setCP(sortA[attackedPos].getCP() - sortA[attackedPos].getCP() / 2);
					}
					else if (sortB[attackerPos].getType() == "Henchmen") {  //attacker is henchmen
						sortB[attackerPos].setCP(sortB[attackerPos].getCP() + 500);
						if (sortA[attackedPos].getCP() < 500) {
							sortA[attackedPos].setCP(0);
						}
						else
						sortA[attackedPos].setCP(sortA[attackedPos].getCP() - 500);
					}
					else if (sortB[attackerPos].getType() == "Soldier") {  //attacker is soldier
						sortB[attackerPos].setCP((height + 1) * 30 + sortB[attackerPos].getCP());
						if (sortA[attackedPos].getCP() < 120) {
							sortA[attackedPos].setCP(0);
						}
						else
						sortA[attackedPos].setCP(sortA[attackedPos].getCP() - 120);
					}
				}
			


			HEAPSORT(sortA);    // after the attack sort the clans again
			HEAPSORT(sortB);


	sortA[toSort].setType("Leader");   // after the sort assign the leader and henchmen types again
	for(int t=1;t<8;t++)
	sortA[toSort - t].setType("Henchmen");

	sortB[toSort].setType("Leader");
	for (int t = 1; t<8; t++)
	sortB[toSort - t].setType("Henchmen");

	
		}

		myfile.close();
		
	}
	else cout << "Unable to open gamelogs file";
	

	unsigned int CPtotA = 0;  //total CPs of clans
	unsigned int CPtotB = 0;
	
	for (int a = 1; a <= toSort; a++) { //calculate the total CPs of clans

		CPtotA += sortA[a].getCP();
		CPtotB += sortB[a].getCP();
	}
	
	if (CPtotA > CPtotB)  // Compare the total CPs of clans to find the winner
		cout << "A Wins"<<endl;
	else
		cout << "B Wins"<< endl;
	
		cout << "At the end of the execution:" <<endl; //print the total CPs of clans at the end of the execution 
		cout << "Clan A has a total of " << CPtotA<<" CPs."<<endl;
		cout << "Clan B has a total of " << CPtotB<<" CPs."<< endl;
		cout << "Difference between amount of the CPs = "<<abs(CPtotA-CPtotB)<<endl;
	
	
	ofstream myoutput;
	myoutput.open("A_results.txt"); // open a new A_results.txt file and write the sorted players of clan A in order
	for (unsigned int t = 1; t <= toSort; t++) {

		myoutput << sortA[t].getId() << "\t" << sortA[t].getCP() << endl;
	}

		

	delete [] sortA;  //memory allocation
	delete [] sortB;
}


    return 0;
}




void MAX_HEAPIFY(Player a[], int i)  // maxheapify the element inside the index on the heap tree
{
	int l, r, largest;
	Player loc;
	l = 2 * i;
	r = (2 * i + 1);
	largest = i;
	if ((l <= heapsize) && a[l].getCP()>a[i].getCP())
		largest = l;
	else
		largest = i;

	if ((r <= heapsize) && (a[r].getCP()>a[largest].getCP()))
		largest = r;

	if (largest != i)
	{
		loc = a[i];
		a[i] = a[largest];
		a[largest] = loc;

		MAX_HEAPIFY(a, largest);
	}
}

void BUILD_MAX_HEAP(Player a[])  //buil max heap tree of the array
{
	heapsize = lengthA;
	for (int i = lengthA / 2; i >= 1; i--)
	{
		MAX_HEAPIFY(a, i);
	}
}

void HEAPSORT(Player a[])  //heapsort the array
{

	BUILD_MAX_HEAP(a);
	int i;
	Player temp;
	for (i = lengthA ; i >= 2; i--)
	{
		temp = a[i];
		a[i] = a[1];
		a[1] = temp;
		heapsize--;
		MAX_HEAPIFY(a, 1);
	}
}
