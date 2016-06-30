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
#include<exp_version.h>
#include<help.txt>

using namespace std;
string trim(const string &t)
{
		int i;
		for(i=0;t[i]==' '||t[i]=='\t';i++);

		return t.substr(i);
}
int main(int argc,char *argv[])
{
		string exp;
		exp_parser e;
		if(argc>1)
		{
			int i=1;
				while(argv[i])
				{
					if(string("--help").compare(argv[i])==0)
							cout<<help_text;
					if(argv[i][0]=='-')
					{
							switch(argv[i][1])
							{
									case 'h':
									cout<<"Usage : exp [-h] [-v] [math-expression] \n";
									return 0;
									case 'v':
									cout<<"exp version " exp_VERSION " by jaisel rahman <jaisel20@gmail.com>\n";
									return 0;
									default:
									cerr<<"Invalid option";
									return 2;
									break;
							}

					}
					if(e.parse(argv[i]))
						cout<<" = "<<e.value<<endl;
					else
					{
						cerr<<" syntax error";
						return 1;
					}
					i++;
				}
		}
		else
		{
			cout<<"Enter an expression to evaluate, q to quit, ? for help \n";
			do
			{
	            cout<<"-> ";
	            getline(cin,exp);
				if(exp=="q")
						return 0;
				else
				if(exp=="?")
						cout<<help_text<<endl;
				else
				if(trim(exp)!="")
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
						cerr<<"  syntax error\n";
					}
				}
			}while(!cin.eof());	
		}
		cout<<"\n";
		return 0;
}


