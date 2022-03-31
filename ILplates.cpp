/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// Jacob Janowski
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>
#include <string.h>

#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//
int ILplates::Hash(string plate)
{

  unsigned long index = 123456789;
  int i = 0;
  int number = 0;
  int digit = 0;
  int space = 0;
  int letter = 0;
  int charNum = 0;
  bool personal = false;
  bool vanityInt = false;
  bool vanityChar = false;

  int plateLength = plate.size();
  
  
  // TODO:
  //check to see if the plate has a space in it. If it does, we have personalized. Else vanity
  //Hash in two different ways.
  //
  //



  //Error checking: 
  //- if the plate is too short/too long, it's invalid.
  //- if the last entry of the plate is a space.
  //- if we encounter a symbol, its invalid.
  //- if we have a lowercase letter, invalid.
  //- if we have a letter after we encounter a space, invalid.
  
  if(plateLength < 1 || plateLength > 8) {
	  return -1;
  }
	
  if(plate.back() == ' ') {
	  //cout << "NO spaces at the EEND" << endl;
	  return -1;
  }  
	  
	while(i < plateLength) {
		
		if ((plate[i] >= 33 && plate[i] <= 47) 
        || (plate[i] >= 58 && plate[i] <= 64)) { 
			return -1;
		}
		
		
		//If we encounter a space, go to next index and add the ascii values of the numbers.
		else if (islower(plate[i]))
		{
			return -1;
			
		}
		//keep track of how many letter and numbers we get.
		else if(isalpha(plate[i])) {
			charNum++;

		}
		else if(isdigit(plate[i])){
			digit++; 

		}
		
		
		//Once we encounter a space, check if there is only 1-2 numbers after it.
		if(plate[i] == ' ') {
			space++;     //keep track of how many spaces.
			personal = true;
			int after = i + 1;
			int tracker = 0;
			
			//If there is a letter after 'space', that's bad.
			if(isalpha(plate[after]) || plate[after] == ' ') { 
				personal = false;
				return -1;
			}
			//make sure that after the space is reached, that only 1-2 numbers exist.
			while(isdigit(plate[after])) {
				tracker++;    //keep track.
				
				if(tracker > 2){
					return -1;
				}
				number = (number + plate[after]) * 123456;		  //index the number.		
				
				after++;

			}
			//If there is a letter after the space, invalid.
			if(isalpha(plate[after])) {
				personal = false;
				return -1;
			}
			
		}
		
		
	  i++;
		
	}
	
	//if we have more than the numbers/letters required, invalid.
	if(digit > 3 || charNum > 7) {
		return -1;
	}
	
	//If the plate is only letters, its vanity.
	if(charNum == plateLength) {
		vanityChar = true;
	}
	//If the plate is all numbers, its vanity.
	else if(digit == plateLength) {
		vanityInt = true;
	}

	
	//If its a vanity of letters: perform specific hashing.
	if(vanityChar) {
		int number2;
		for (int j = 0; j < charNum; j++)
		{
			number2 = plate[0]  - 4320;
			index = (((number2 + plate[j]) - 62441) * (j + 1)) - 123456;	

			
		}
		
		
	}
	
	//if its a personal plate: hash accordingly.
	else if(personal) {
		for(int i = 0; i < plateLength; i++) {
			
            if (plate[i] == '0')
            {
                    index = (i + 1) + 73432;
            }
			else if (plate[i] == ' ')
            {
				index = index + (i + 1) * 242 + (HT.Size() / 32);
            } 
				
	
			index = (((number - 432) * 123456) / 3 ) * (i + 1) * 987654;
			

		
		
		}
	}
	
	//If its a vanity of numbers: hash accordingly.
	else if(vanityInt) {
		//int firstLetter = plate[0] * 9876213;
		int firstLetter = plate[0] * 9876213;
		index = (stoi(plate) * firstLetter) - (firstLetter / 7);
		//index = stoi(plate) * firstLetter;
		
	}
	
  return index % HT.Size();
}


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
  //
  // TODO:
  //
  
  int bucket = Hash(plate);
  int bucketsProbed = 0;
  bool empty;
  string key;
  int value;
  
  HT.Get(bucket, empty, key, value); //retrieve plate


  //Loop until we find a valid plate.
  while((!empty) && (HT.Size() > bucketsProbed)) {
	  
	  HT.Get(bucket, empty, key, value);
	  
	  if(key == plate) {
		  
		  return value;
	  }
	  
	  bucket = (bucket + 1) % HT.Size();
	  bucketsProbed++;
	  
	  
  }

 
  
  return -1;
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
  //
  // TODO:
  bool empty;
  string key;
  int value;
  int bucketsProbed = 0;
  int bucket = Hash(plate);

  if(bucket == -1) {
	  return;
  }

  //Get the plate info and insert.
  //If we have the same plate, add the fines together and update.
  HT.Get(bucket, empty, key, value);
	
  while(bucketsProbed < HT.Size()) {
	  
	  //as long as the index is empty or the plate matches a preexisting one, set the new values.
	  if((empty) || key == plate) {	  
		  HT.Set(bucket, plate, newValue);		  
		  return;
	 	}
	  
	  bucket = (bucket + 1) % HT.Size();
	  bucketsProbed++;
	  
	  //Get another plate/fine.
	  HT.Get(bucket, empty, key, value);
    }
	return;
}
