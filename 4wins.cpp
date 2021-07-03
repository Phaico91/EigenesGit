#include <fstream> 
#include <sstream> 
#include <string> 
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <ctime>
#include <cstddef> 
#include <cassert>
using namespace std;

struct spieler
{
	string name;
	char symb;
}player;

void drawfield(char field[8][8])
{
	for(int i=0;i<8;i++)
	{
		printf("%d    ",i+1);
		for(int o=0;o<8;o++)
		{
			printf("%c    ",field[i][o]);
			if(o==7)
			{
				printf("\n");
			}
		}
	}
	printf("     1    2    3    4    5    6    7    8    \n");
}

void fillfield(char field[8][8])
{
	for(int i=0;i<8;i++)
	{
		for(int o=0;o<8;o++)
		{
			field[i][o]='#';
		}
	}
}

void turnmsg(string name)
{
	cout << name <<" ist dran"<< endl;
}
bool isPosfree(char field[8][8],int y)
{
	assert(y>=0&&y<=8);
	for(int i=7;i>=0;i--)
	{ 
		if(field[i][y-1]=='#')
		{
			
			return true;
		}
	}
	return false;
	
}

bool setOnPos(char field[8][8],char symb, int y)
{
	for(int i=7;i>=0;i--)
	{
		if(field[i][y-1]=='#')
		{
			field[i][y-1]=symb;
			return true;
		}
	}
	return false;
	
}



spieler setup_player(string _name,char _symb)
{
	spieler p1;
	p1.name=_name;
	p1.symb=_symb;
	return p1;
}

int game_end(char field[8][8])
{
  int count1 = 0, count2 = 0;
  for (int i = 0; i <=7; ++i)
  {
    for (int j = 0; j <=7; ++j)
    {
      if (field[i][j] == 'x')
        ++count1, count2 = 0;
      else if (field[i][j] == 'o')
        ++count2, count1 = 0;
      else
        count1 = count2 = 0;
      if (count1 >= 4)
        return 1;
      if (count2 >= 4)
        return 2;
    }
  }
   for (int j = 0; j <=7; ++j)
  {
    for (int i = 0; i <=7; ++i)
    {
      if (field[i][j] == 'x')
        ++count1, count2 = 0;
      else if (field[i][j] == 'o')
        ++count2, count1 = 0;
      else
        count1 = count2 = 0;
      if (count1 >= 4)
        return 1;
      if (count2 >= 4)
        return 2;
    }
  }
  count1 = 0, count2 = 0;
  for( int k = 0 ; k < 8 * 2 ; k++ ) {
        for( int j = 0 ; j <= k ; j++ ) {
            int i = k - j;
            if( i < 8 && j < 8 ) {
                if (field[i][j] == 'x')
        ++count1, count2 = 0;
      else if (field[i][j] == 'o')
        ++count2, count1 = 0;
      else
        count1 = count2 = 0;
      if (count1 >= 4)
        return 1;
      if (count2 >= 4)
        return 2;
            }
        }
       
    }
	//obere rechte hälfte + Hauptdiagonale
    count1 = 0, count2 = 0;
	int temp=0;
  for( int k = 0 ; k < 8 ; k++ ) {
	  {
		  temp=0;
        for( int j = 0 ; j < 8 ; j++ ) {
            //int i = k - j;
            if( k < 8 && j < 8 ) {
                if (field[j][k+temp] == 'x')
        ++count1, count2 = 0;
      else if (field[j][k+temp] == 'o')
        ++count2, count1 = 0;
      else
        count1 = count2 = 0;
      if (count1 >= 4)
        return 1;
      if (count2 >= 4)
        return 2;
            }	
			temp++;
	  }
	  }
       
	   //untere linke hälfte ohne Hauptdiagonale
	       count1 = 0, count2 = 0;
	int temp=0;
  for( int k = 0 ; k < 8 ; k++ ) {
		  temp=0;
        for( int j = 0 ; j < 8 ; j++ ) {
            //int i = k - j;
            if( k < 8 && j < 8 ) {
                if (field[k+temp][j] == 'x')
        ++count1, count2 = 0;
      else if (field[j][k+temp] == 'o')
        ++count2, count1 = 0;
      else
        count1 = count2 = 0;
      if (count1 >= 4)
        return 1;
      if (count2 >= 4)
        return 2;
            }	
			temp++;
	  }
	  }
	   return 0;
    }
       
    
    
 
  return 0;
}

int main(void){
	bool game=true;
spieler p1,p2;
int i=0,temp;
char field[8][8];
char p1_s,p2_s;
 char symb[2];
 string namen[2]={"Spieler1","Spieler2"};
fillfield(field);

drawfield(field);


cout << "Willkommen zu Vier-Gewinnt auf der Konsole\nSpieler 1, spielst du O oder X?";
cin >> symb[0];


if(symb[0]=='X'||symb[0]=='x')
{
	symb[0]='x';
	symb[1]='o';
}
else{
	symb[0]='o';
	symb[1]='x';
}
p1=setup_player(namen[0],symb[0]);
p2=setup_player(namen[1],symb[0]);
while(game){
	turnmsg(namen[i]);
	cout << "mache deinen Zug, "<< namen[i];
	cin >> temp;
	if(isPosfree(field,temp))
	{
		
		setOnPos(field,symb[i],temp);
		if(i==0)
	{
		i=1;
	}
	else{
		i=0;
	}
	}
	else{
	while(!isPosfree(field,temp))
	{
		cout << "mache deinen Zug, "<< namen[i];
	cin >> temp;
	if(isPosfree(field,temp))
	{
		
		setOnPos(field,symb[i],temp);
	}
	}if(i==0)
	{
		i=1;
	}
	else{
		i=0;
	}}
	
	
	drawfield(field);
	if(game_end(field)==1||game_end(field)==2)
	{
		cout << "Spiel vorbei";
		game=false;
	}
	
}


}