#include <fstream> 
#include <sstream> 
#include <string> 
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <ctime>
#include <cstddef> 
#include <cassert>
using namespace std;

void printpositions(string field[3][3])
{
	for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				cout << "   ["<< i <<"," <<o << "]";	
				if(o==2)
				{
				printf("\n");
				}	
			}				
		}
}

void printfield(string field[3][3])
{
		for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				cout << "   "<<field[i][o];	
				if(o==2)
				{
				printf("\n");
				}	
			}				
		}
}

void fillfield(string field[3][3])
{
	int p=1;
		for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				field[i][o]=to_string(p);
				p++;		
			}}
}

bool isPosfree(string field[3][3],int pos)
{
	
	for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]!="X"&&field[i][o]!="O"&&field[i][o]==to_string(pos))
				{
					return true;
				}	
			}}
	
	return false;
	
}


void setOnPos(string field[3][3],int pos,string symb)
{
	
	for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]!="X"&&field[i][o]!="O"&&field[i][o]==to_string(pos))
				{
					field[i][o]=symb;
				}	
			}}
}

void setPlayers(string player[3][2])
{
	 player[0][0]="Player 1";
	 player[1][0]="Player 2";
	 player[2][0]="Computer";
	 player[0][1]="X";
	 player[1][1]="O";
	 player[2][1]="#";
}

int nextturn(int turn,int mode)
{
	if(mode==1)
	{
		if(turn==0)
		{
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if(turn==0)
		{
			return 2;
		}
		else{
			return 0;
		}
	}
}

bool draw(string field[3][3])
{
	for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]!="X"&&field[i][o]!="O")
				{
					return false;
				}	
			}}
			return true;
}

int won(string field[3][3])
{
	int count1 = 0, count2 = 0,count3=0;
		for(int i=0;i<3;i++)
		{	
	count1 = 0, count2 = 0,count3=0;
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]=="X")
				{
					++count1, count2 = 0,count3=0;
				}
				else if(field[i][o]=="O")
				{
					 ++count2, count1 = 0,count3=0;
				}
				else if(field[i][o]=="#")
				{
					 ++count3,count2=0, count1 = 0;
				}
				else{
					 count1 = count2 =count3 = 0;
				}
				 if (count1 >= 3)
					return 1;
				if (count2 >= 3)
					return 2;
				if (count3 >= 3){
				cout << "zeile\n";
				return 3;}
			}}
	
	
	count1 = 0, count2 = 0,count3=0;
		for(int o=0;o<3;o++)
		{	
	count1 = 0, count2 = 0,count3=0;
			for(int i=0;i<3;i++)
			{	
				if(field[i][o]=="X")
				{
					++count1, count2 = 0,count3=0;
				}
				else if(field[i][o]=="O")
				{
					 ++count2, count1 = 0,count3=0;
				}
				else if(field[i][o]=="#")
				{
					 ++count3,count2=0, count1 = 0;
				}
				else{
					 count1 = count2 = count3 =0;
				}
				 if (count1 >= 3)
					return 1;
				if (count2 >= 3)
					return 2;
				if (count3 >= 3){
				cout << "spalte\n";
				return 3;
				}
			}}
			
			if((field[0][0]=="O"&&field[1][1]=="O"&&field[2][2]=="O")||(field[2][0]=="O"&&field[1][1]=="O"&&field[0][2]=="O"))
			{
				return 1;
			}
			if((field[0][0]=="X"&&field[1][1]=="X"&&field[2][2]=="X")||(field[2][0]=="X"&&field[1][1]=="X"&&field[0][2]=="X"))
			{
				return 2;
			}
			if(field[0][0]=="#"&&field[1][1]=="#"&&field[2][2]=="#")
			{
				cout << "diag\n";
				return 3;
			}
			if(field[2][0]=="#"&&field[1][1]=="#"&&field[0][2]=="#")
			{
				cout << "antidiag\n";
				return 3;
			}
	return 0;
}

string onPos(string field[3][3],int pos)
{
	for(int i=0;i<3;i++)
		{	
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]=="X")
				{
					return "X";
				}
				else
				{
					return "0";
				}
			
			}}
			return"0";
}

int s2inZeilen(string field[3][3])
{
	int count1 = 0;
		for(int i=0;i<3;i++)
		{	
		count1=0;
			for(int o=0;o<3;o++)
			{	
				if(field[i][o]=="X")
				{
					++count1;
				}
				if(field[i][o]=="#")
				{
						return -1;
				}
				 if (count1 >= 2)
					return i;
				
			}
			}
			return -1;
}

int s2inSpalten(string field[3][3])
{
	int count1 = 0;
		for(int i=0;i<3;i++)
		{	
	count1=0;
			for(int o=0;o<3;o++)
			{	
				if(field[o][i]=="X")
				{
					++count1;
				}
				if(field[o][i]=="#")
				{
						return -1;
				}
				 if (count1 >= 2)
					return o;
				
			}}
			return -1;
}

bool s2inAntiDiag(string field[3][3])
{
	int count1 = 0;
		for(int i=0;i<3;i++)
		{	
				count1=0;
				if(field[2-i][i]=="X")
				{
					++count1;
				}
				else if(field[2-i][i]=="#")
				{
					return false;
				}
				else{
					count1=0;
				}
				 if (count1 >= 2)
					return true;
				
			}
			return false;
}

bool s2inDiag(string field[3][3])
{
	int count1 = 0;
		for(int i=0,o=0;i<3;i++,o++)
		{	
				count1=0;
				if(field[i][o]=="X")
				{
					++count1;
				}
				if(field[i][o]=="#")
				{
					return false;
				}
				 if (count1 >= 2)
					return true;
				
			}
			return false;
}

void setzeInZeile(string field[3][3],int zeile)
{
	
	for(int i=0;i<3;i++)
	{
		if(field[zeile][i]!="X"&&field[zeile][i]!="#")
		{
			field[zeile][i]="#";
			return;
		}
	}
}
void setzeInSpalte(string field[3][3],int spalte)
{
	for(int i=0;i<3;i++)
	{
		if(field[i][spalte]!="X"&&field[i][spalte]!="#")
		{
			field[i][spalte]="#";
			return;
		}
	}
}
void setzeInDiag(string field[3][3])
{
	for(int i=0,o=0;i<3;i++,o++)
	{
		if(field[i][o]!="X"&&field[i][o]!="#")
		{
			field[i][o]="#";
			return;
		}
	}
}
void setzeInAntiDiag(string field[3][3])
{
	for(int i=2,o=0;i>=0;i--,o++)
	{
		if(field[i][o]!="X"&&field[i][o]!="#")
		{
			field[i][o]="#";
			return;
		}
	}
}

bool compCounter(string field[3][3])
{
		if(s2inZeilen(field)>=0)
		{
			//setzeInZeile(field,s2inZeilen(field));
			return true;
		}
		if(s2inSpalten(field)>=0)
		{
			//setzeInSpalte(field,s2inSpalten(field));
			return true;
		}
		 if(s2inAntiDiag(field))
		{
			//setzeInAntiDiag(field);
			return true;
		} 
		if(s2inDiag(field))
		{
			//setzeInDiag(field);
			return true;
		}
		return false;
}
void docompCounter(string field[3][3])
{
		if(s2inZeilen(field)>=0)
		{cout <<"Zeile NR: " <<s2inSpalten(field)<< "\n";
			setzeInZeile(field,s2inZeilen(field));
			return;
		}
		if(s2inSpalten(field)>=0)
		{ cout <<"Spalte NR: " <<s2inSpalten(field)<< "\n";
			setzeInSpalte(field,s2inSpalten(field));
			return;
		}
		 if(s2inAntiDiag(field))
		{
			setzeInAntiDiag(field);
			return;
		} 
		if(s2inDiag(field))
		{
			setzeInDiag(field);
			return;
		}
		
}

int main(void){
	string field[3][3];
	string player[3][2];
	srand(time(NULL));
	int temp;
	fillfield(field);
	setPlayers(player);
	int turn=0;
	int mode;
	bool gameOver=false;
	cout << "Willkommen zu Tic Tac Toe, welchen Modus willst du spielen?\n(1)\t2 Spieler\n(2)\tGegen Computer";
	cin >> mode;
	printfield(field);
	//printpositions(field);
	while(!gameOver)//gameloop
	{
		if(turn==0||turn==1){
		cout << "Mache deinen Zug, " << player[turn][0] << "  ";
		cin >> temp;
		if(isPosfree(field,temp))
		{
			setOnPos(field,temp,player[turn][1]);
			printfield(field);
			if(won(field)!=0)
			{
			cout << "Gameover, " << player[turn][1] << " hat gewonnen!\n";
			gameOver=true;
			}
		}
		turn=nextturn(turn,mode);
		}
		else{
			cout << "Computer zieht\n";
			
			if(compCounter(field)==false)
			{
				cout << "Computer spielt frei\n";
					temp = rand() % 8 + 1;
				if(isPosfree(field,temp))
				{
					setOnPos(field,temp,player[turn][1]);
				}
				else{
				while(!(isPosfree(field,temp)))
				{
					temp = rand() % 8 + 1;
				}}
				setOnPos(field,temp,player[turn][1]);
			}
			else{
				cout << "Computer kontert\n";
			docompCounter(field);			
			}
			printfield(field);
			if(won(field)!=0)
			{
			cout << "Gameover, " << player[turn][0] << " hat gewonnen!\n";
			gameOver=true;
			}
			turn=nextturn(turn,mode);
			
		}
		
		if(draw(field)){
			cout << "Gameover,Unentschieden\n ";;
			gameOver=true;
		}
	}
	
	printfield(field);
}