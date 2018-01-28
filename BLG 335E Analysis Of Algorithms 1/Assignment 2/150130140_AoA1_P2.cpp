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
#include <bits/stdc++.h>//count


using namespace std;



class Word{   // Object for only one word
	string word;    //contains the word and its order
	int wordOrder;
public:
	void setWord(string a){word=a;};
	string getWord(){return word;};
	void setWordOrder(int a){wordOrder=a;};
	int getWordOrder(){return wordOrder;};
	Word operator=(Word a){   // = asignment overload
		this->setWord(a.getWord());
		this->setWordOrder(a.getWordOrder());
		return *this;
	}
};


class NovelLine{ // Object for a single line 
	int id;			//it contains id, wordcount and a pointer to words it has
	int wordCount;
public:
	Word* words;
	void setId(int a){id=a;};
	int getId(){return id;};
	void setWordCount(int a){wordCount=a;};
	int getWordCount(){return wordCount;};
    	NovelLine operator=(NovelLine a){   // = asignment overload
    		this->setId(a.getId());
    		this->setWordCount(a.getWordCount());
    		this->words=a.words;
    		return *this;
    	} 
    };

    int quicksortCounter = 0;  //counter and variables for assignment
    int N;
    int M;
    int partition(NovelLine arr[],int p,int r);   //sort functions for NovelLine objects
    void quickSort(NovelLine arr[],int p,int r);
    
    int rPartition(Word arr[],int p,int r);    //sort functions for Word objects
    void rQuickSort(Word arr[],int p,int r);
    int wPartition(Word arr[],int p,int r);
    
    int partition(int arr[],int p,int r);		//sort functions for  integer values
    void quickSort(int arr[],int p,int r);




int main(int argc,char *agv[]){


if(atoi(agv[1])==1) {  //choose of which operation will be carried on program, novel or number sort
					// 1  so we sort novel lines
    		M= atoi(agv[2]);
    		N= atoi(agv[3]);

    		NovelLine * lines ;  //pointer to save lines

ifstream myfile ;             //read from mobydick.txt
myfile.open("mobydick.txt");
string countline;
string line;
int k=0; //number of lines

if (myfile.is_open())
{      

	while(getline(myfile,countline)) // calculation of number of lines
		k++;

	lines= new NovelLine[k];   // initiaton of the array

	myfile.clear();             //reset the file cursor
	myfile.seekg(0,myfile.beg);

	int i=0;
	while(getline(myfile,line)){    //reads a whole line 


		istringstream iss (line);
		string temp;
		
		iss>>temp;
		lines[i].setId(atoi(temp.c_str()));	
		iss>>temp;
		

		lines[i].setWordCount(count(temp.begin(),temp.end(),'_')+1);  // finds the number of words a line has 
		lines[i].words= new Word[lines[i].getWordCount()]; 			//initiate word array of line object

		int k = 0;
		int a = 1;
		string part;               //reads words

		while(temp[a]!='}')
		{
			if(temp[a]=='_')
			{
				lines[i].words[k].setWord(part.c_str());
				k++;
				part="";
			}
			else
				part+=temp[a];
			a++;
		}
		lines[i].words[k].setWord(part.c_str());

		iss>>temp;

		k = 0;
		a = 1;
		part="";				//reads wordorders

		while(temp[a]!='}')
		{
			if(temp[a]=='_')
			{
				lines[i].words[k].setWordOrder(atoi(part.c_str()));
				k++;
				part="";
			}
			else
				part+=temp[a];	
			a++;
		}
		lines[i].words[k].setWordOrder(atoi(part.c_str()));
		i++;
	}


	quickSort(lines,0,k-1); //sorts lines by id

	for(int a = 0;a<k;a++)   //sords words of each line by word orders
		rQuickSort(lines[a].words,0, lines[a].getWordCount()-1);

}

else cout << "Unable to open file"; 


ofstream myoutput;     // opens a new novel.txt file and writes the lines in correct order
myoutput.open("novel.txt");

for(unsigned int t=0;t<k;t++){
		
	for(int y =0;y<lines[t].getWordCount();y++)
		myoutput<<lines[t].words[y].getWord()<<" ";
		
	myoutput<<endl;
}

for(int end=0;end<k;end++)  //memory allocation 
	delete [] lines[k].words;
delete [] lines;

}
else{ //atoi(agv[1])==2, second part of the program

	int toSort= atoi(agv[2]);  //number of inputs to sort

int* sort = new int[toSort];  // array initiation for numbers to sort

  ifstream myfile ;             //read from numbers_to_sort.txt
  myfile.open("numbers_to_sort.txt");
  string num;
  if (myfile.is_open())
  {      
  	int i=0;
    while( getline (myfile,num) && i<toSort)  //reads each line and assigns it to a number inside array
    {
    	
    	istringstream iss (num);
    	string temp;
    	
    	iss>>temp;
    	sort[i]=atoi(temp.c_str());
    	
    	i++;        
    }
    
    myfile.close();   
}

else cout << "Unable to open file"; 

quickSort(sort,0,toSort-1); //sorts the number array



ofstream myoutput;     // opens a new numbers.txt file and writes the sorted numbers as required 
myoutput.open("numbers.txt");

for(unsigned int t=0;t<toSort;t++){
	
	myoutput<<sort[t]<<endl;
}

delete [] sort;  //memory allocation

}

return 0;
}



void quickSort(NovelLine arr[],int p,int r){    // Quicksort function for lines object that sorts the lines by their ids 
	quicksortCounter++;
	
	
	if(quicksortCounter==M){
		
		cout<<arr[N].getId()<<endl;
		
	}
	if(p<r){
		int q=partition(arr,p,r);
		quickSort(arr,p,q-1);
		quickSort(arr,q+1,r); 

	}
}


int partition(NovelLine arr[],int p,int r){   // Partition function for lines object that compares the lines by their ids 
	int x = arr[r].getId();
	int i= p-1;
	NovelLine temp[1];
	for(int j=p;j<r;j++)
	{
		if(arr[j].getId()<=x)
		{
			i++;
			temp[0]=arr[i];
			arr[i]=arr[j];
			arr[j]=temp[0];
		}	
	}
	temp[0]=arr[i+1];
	arr[i+1]=arr[r];
	arr[r]=temp[0];
	return 1+i;
}


void rQuickSort(Word arr[],int p,int r){   // Quicksort function for word object that sorts the words by their word orders 

	if(p<r){
		int q=rPartition(arr,p,r);
		rQuickSort(arr,p,q-1);
		rQuickSort(arr,q+1,r); 
	}

}


int rPartition(Word arr[],int p,int r){  //Randomized partition function for Word object that choses pivot by rand()
	srand(255);
	int i = p + (rand()%(int)(r-p+1));
	Word temp[1];
	temp[0]=arr[i];
	arr[i]=arr[r];
	arr[r]=temp[0];
	
	return wPartition( arr, p, r);
}


int wPartition(Word arr[],int p,int r){  // Partition function for Word object that compares the wrds by their word orders 
	int x = arr[r].getWordOrder();
	int i= p-1;
	Word temp[1];
	for(int j=p;j<r;j++)
	{
		if(arr[j].getWordOrder()<=x)
		{
			i++;
			temp[0]=arr[i];
			arr[i]=arr[j];
			arr[j]=temp[0];
		}		
	}
	temp[0]=arr[i+1];
	arr[i+1]=arr[r];
	arr[r]=temp[0];
	return 1+i;
}


void quickSort(int arr[],int p,int r){  // Quicksort function for integer values

	if(p<r){
		int q=partition(arr,p,r);
		quickSort(arr,p,q-1);
		quickSort(arr,q+1,r); 
	}

}


int partition(int arr[],int p,int r){  // Partition function for integer values
	int x = arr[r];
	int i= p-1;
	int temp;
	for(int j=p;j<r;j++)
	{
		if(arr[j]<=x)
		{
			i++;
			temp=arr[i];
			arr[i]=arr[j];
			arr[j]=temp;
		}	
	}
	temp=arr[i+1];
	arr[i+1]=arr[r];
	arr[r]=temp;
	return 1+i;
}


