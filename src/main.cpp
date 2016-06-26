/**
*	
*	Expression Parser 
*		- to evaluate simple math expression
*
*	written in c++ 
*	
*
*	by   jaisel rahman
*		<jaisel20@gmail.com>
*
**/


#include<iostream>
#include<string>
#include<exp_parser.h>
using namespace std;


int main(int argc,char *argv[])
{
		string exp;
		exp_parser e;
		if(argc>=2)
		{
			int i=1;
			while(argv[i])
			{
				if(e.parse(argv[i++]))
					cout<<" = "<<e.value<<endl;
				else
					cout<<" syntax error";
			}
		}
		else
		{
			char c[1024];
			cout<<"Enter an expression to evaluate, q to quit, ? for help \n";
			do
			{
				cout<<"-> ";
		    	cin.getline(c,1024,'\n');
	            exp=c;
	       	}while(exp=="");
			while(exp!="q")
			{
				if(e.parse(exp))
				{
						cout<<" = "<<e.value<<endl;
				}
				else
				{
					cout<<"   ";
					cout.width(e.errorpos);
					cout.fill('~');
					cout<<'^'<<endl;
					cout<<"  syntax error\n";
				}
				do
				{
				cout<<"-> ";
				cin.getline(c,1024,'\n');
				exp=c;
				}while(exp=="");
			}
		}

		return 0;
}


