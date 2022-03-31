/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:  Jacob Janowski
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <cassert>

#include "ILplates.h"

using namespace std;


//Create a vector of structs; include the plate and fine.
struct hashStruct {
	
	string plate;
	int fine;
};

//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //

  while (!infile.eof())
  {
    getline(infile, plate);

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }
		

    }//valid

    getline(infile, fine);
  }
}




int main()
{
  int    N;        // = 10000;
  string basename; // = "tickets1";
  
	
  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;
	
  //create the output file to write to.
  string of = basename + "-output.txt";
  ofstream outfile(of);

  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);
  
  //
  // debugging:
  //
  //Initialize struct.
  hashStruct hashValue;

  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();
  vector<hashStruct>hashVec;    //create vector of structs.

  //Loop throught and pass in the plate number and fine amount.
  //Push back into vector.
  for(unsigned int x = 0; x < plates.size(); x++) {
	  hashValue.plate = plates[x];
	  hashValue.fine = amounts[x];  
	  hashVec.push_back(hashValue);

  }
	  
	int n = hashVec.size();

    int i, j;  
    string tempPlate;
    int tempFine;
    
    cout << "Sorting..." << endl;

   //Insertion sort: sort according to plate name and 
   //attach the corresponding fines.
    string key;
    for (i = 1; i < n; i++) 
    {  
        key = hashVec[i].plate;  
        tempFine = hashVec[i].fine;
        j = i - 1;  
  
        /* Move elements from vector to one position ahead  
        of their current position */
        while (j >= 0 && hashVec[j].plate > key) 
        {  
            hashVec[j + 1].plate = hashVec[j].plate;  
            hashVec[j + 1].fine = hashVec[j].fine;
            j = j - 1;  
        }  
        hashVec[j + 1].plate = key;
        hashVec[j + 1].fine = tempFine;  
    }

       
  //Write to the file in ascending order.
  cout << "Writing '" << of << "'...'" << endl;

  for ( const hashStruct x : hashVec )
  {
     outfile << "\"" << x.plate << "\"" << " $" << x.fine << endl;
  }
	  
	


  //
  // done:
  //
  return 0;
}