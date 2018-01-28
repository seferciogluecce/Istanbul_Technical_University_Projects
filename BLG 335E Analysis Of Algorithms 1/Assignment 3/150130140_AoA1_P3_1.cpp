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
#include<string>
#include <sstream>  //isstream
#include<time.h>


using namespace std;



class Number {                   // Object for a Number
	string id;               //contains the id of the number
	long long number;	 // contains the number 

public:

	void setId(string a) { id = a; };  //get - set functions
	string getId() { return id; };

	void setNumber(long long a) { number = a; };
	long long getNumber() { return number; };

	Number operator=(Number a) {      // = asignment overload
		this->setId(a.getId());
		this->setNumber(a.getNumber());
		return *this;
	}
};


void radixSort(Number arr[], int n);
void countingSort(Number arr[], int n, int exp);


int main(int argc, char *agv[]) {

	clock_t t;   // initiation for calculation of execution time 
	t=clock();
	
	unsigned int toSort= atoi(agv[1]);  //number of inputs to sort, read from terminal

	Number* sort = new Number[toSort];  // array initiation for numbers to sort

	ifstream myfile;             //read from numbers_to_sort.txt
	myfile.open("numbers_to_sort.txt");
	string num;
	if (myfile.is_open())
	{
		unsigned int i = 0;
		while (getline(myfile, num) && i<toSort)  //reads each line and assigns it to a number inside array
		{

			istringstream iss(num);
			string temp;

			iss >> temp;
			sort[i].setId(temp.c_str());            // assign id

			iss >> temp;
			sort[i].setNumber(atoll(temp.c_str())); // assign number value

			
			i++;
		}

		myfile.close();
	}

	else cout << "Unable to open file";

	radixSort(sort, toSort); //sort the number array using radixsort



	ofstream myoutput;     // open a new sorted_numbers.txt file and write the sorted numbers as required 
	myoutput.open("sorted_numbers.txt");

	for (unsigned int t = 0; t<toSort; t++) {

		myoutput << sort[t].getId() << "\t" << sort[t].getNumber() << endl;
	}


	delete[] sort;  //memory allocation
	

	t=clock()-t;

	cout<< " Execution time " << ((float)t)/CLOCKS_PER_SEC << " seconds."<< endl; //print the execution time

	
	
	return 0;
}



void countingSort(Number arr[],  int n, int exp)  //sorts the indexes of the given digit 

{
	Number* B = new Number[n];
	

	 int i, C[10] = { 0 };

	for (i = 0; i < n; i++)

		C[(arr[i].getNumber() / exp) % 10]++;

	for (i = 1; i < 10; i++)

		C[i] += C[i - 1];

	for (i = n - 1; i >= 0; i--)

	{

		B[C[(arr[i].getNumber() / exp) % 10] - 1] = arr[i];
		C[(arr[i].getNumber() / exp) % 10]--;

	}

	for (i = 0; i < n; i++)

		arr[i]=B[i];

	
	delete[] B;
}


void radixSort(Number arr[],  int n)  //decides which digit to sort and uses counting sort to sort that digit of the numbers

{
	Number max = arr[0];
	

	for (unsigned int i = 1; i < n; i++)

		if (arr[i].getNumber() > max.getNumber())

			max = arr[i];


	for (int exp = 1; max.getNumber() / exp > 0; exp *= 10)
		countingSort(arr, n, exp);

}




