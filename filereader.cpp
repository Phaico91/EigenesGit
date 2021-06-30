#include <fstream> 
#include <sstream> 
#include <string> 
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <ctime>
#include <cstddef> 

using namespace std; 
int get_rand_num(int limit){
        return rand()%limit;}
		


int main(void){ 
	srand((unsigned)time(0)); 
	string filename="wortliste.txt",filler="-";
	bool another=true,geloest=false,gameover=false;
	const int versuche=11;
	size_t fails=0;
	char vorschlag;
	string unseen="_";
	unsigned int richtiggeratene;
	
	int number_of_lines=0; 
	
	ifstream input(filename);

	if(!input) {
		cerr << "Beim Oeffnen der Datei " << filename << "\n";
	return EXIT_FAILURE;
	}

	string line; while(getline(input,line)){
		number_of_lines++;
	}
	string* content= new string[number_of_lines];
	 ifstream input2(filename); 
	int o=0;
	 while(getline(input2,line)){
		content[o]=line; o++;}
	cout << "Anzahl Worte: " << number_of_lines << endl; 
	while(another)
	{
	fails=0;
	char next='y';
	int randindex=get_rand_num(number_of_lines -1); 
	//cout << "Zufaelliges Wort ist: " << content[randindex] << endl;
	int count=content[randindex].size();
	richtiggeratene=0;
	string* str=new string[count];
	for(int i=0;i<count;i++){
		str[i]=unseen;
	}
	// HIER MUSS NOCH die spiellogik gemacht werden
	for(int i=0;i<count;i++){
		cout << str[i] << " ";
	}
	cout << " " << endl;
	while(!geloest&&!gameover){
		cout << "Schlage ein Buchstaben vor: ";
		cin >> vorschlag;
	
	if(content[randindex].find(vorschlag)!= std::string::npos){
		
		
		while(content[randindex].find(vorschlag)!= std::string::npos){
			int temp= content[randindex].find(vorschlag);		
			str[temp]=vorschlag;
			richtiggeratene++;
			content[randindex].replace(temp,1,filler);
			for(int i=0;i<count;i++){
				cout << str[i] << " ";
			}
			cout << " " << endl;
			//cout << count << endl;
			//cout << richtiggeratene << endl;
			
			
			
		}
	}
	else{
		fails++;
	}
	
	
	if(richtiggeratene>=count)
	{
		geloest=true;
	cout << "WIN!" << endl;}
	if(fails>=versuche){
			gameover=true;
			cout << " LOOSE!" << endl;
		}	
	}
	
	cout << "Neues Wort? Y/N ";
	cin >> next;
	if(next=='y'||next=='Y'){
		another=true;
		gameover=false;
		geloest=false;
	}
	else if(next=='n'||next=='N'){
		another=false;
	}
	else{
		another=false;
	}
	
	}
}

