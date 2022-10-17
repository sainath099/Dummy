//program to define the class phonebook
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <phonebook.h>

using namespace std;


//default constructor 
Phonebook::Phonebook(){};


//parameterised constructor
Phonebook::Phonebook(string name , string phonenumber ){
      this->name = name;
      this->phonenumber = phonenumber ;
};


//function to store the data to the phonebook
bool Phonebook::ToStoreData(string filename){
      ofstream outfile ;
      if(filename != ""){
            outfile.open("/home/cguser11/phonebook_management/db/"+filename + ".txt", ios_base::app);
            string contact = name + "," + phonenumber ;
            outfile  << contact << "\n";
            outfile.close();
            return true;
       }
       else{
       	return false;
       }

}


//function to remove data from the phonebook
string Phonebook::ToRemoveData(vector<string> afterRemoved,string filename){
      ofstream outfile ;
      if(filename != ""){
      	outfile.open("/home/cguser11/phonebook_management/db/"+filename + ".txt");
      	if(outfile.is_open()){
      		for(auto contact : afterRemoved){
      	      		outfile  << contact <<  "\n";
      	      }
      	      return "yes" ;
      	}
      	outfile.close();
     	}
      	return "" ;
}


