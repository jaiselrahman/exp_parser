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
#include<cmath>
#include<exp_parser.h>
#include"help.txt"
#include<mpreal.h>

using namespace mpfr;
using namespace std;

typedef  mpreal type;

string trim(const string &t)
{
		int i;
		for(i=0;t[i]==' '||t[i]=='\t';i++);
		return t.substr(i);
}
int main(int argc,char *argv[])
{
		string exp;
		exp_parser<type> e;
		int n=6;
		mpreal::set_default_prec(mpfr::digits2bits(n));
		e.add_var("pi",mpreal("3.14"),exp_parser<type>::type::constant);
		if(argc>1)
		{
			bool cl=false;
			int i=1;
				while(argv[i])
				{
					if(string("--help").compare(argv[i])==0)
					{
							cout<<help_text;
							return 0;
					}
					else
					if(argv[i][0]=='-')
					{
							switch(argv[i][1])
							{
									case 'p': 
									if(!argv[i][2])
									{
											
											cout<<"Option -p requires valid integer argument\n";
											return 1;
									}
									else
									{
											n=strtol(&argv[i][2],NULL,10)+1;
											mpreal::set_default_prec(mpfr::digits2bits(n));
											i++;
									}
									continue;
									
									case 'h':
									cout<<"Usage : exp [-h] [-v] [math-expression] \n";
									cout<<"Try 'exp --help' for more details\n";
									return 0;
									break;
									
									case 'v':
									cerr<<"exp version " exp_VERSION " by jaisel rahman <jaisel20@gmail.com>\n";
									return 0;
									break;

									default:
									cerr<<"Invalid option\n";
									cerr<<"Try 'exp --help for more details\n";
									return 2;
									break;
							}

					}
					if(e.parse(argv[i]))
					{
							cl=true;
							cout.precision(n);
							cout<<" = "<<e.value<<'\n';
					}
					else
					{
							cerr<<" syntax error";
						return 1;
					}
					i++;
				}
				if(cl) return 0;
		}
		
			cout<<"Enter an expression to evaluate, q to quit, ? for help \n";
			do
			{
	            cout<<"-> ";
				getline(cin,exp,'\n');
				
				if(exp=="q")
				{
						return 0;
				}
				else if(exp=="?")
				{
						cout<<help_text<<endl;
				}
				else if(trim(exp)!=""&&!cin.eof())
				{
					if(e.parse(exp))
					{
					 		cout.precision(n);
							cout<<" = "<<e.value<<'\n';
					}		
					else
					{
							cout<<"   ";
							cout.width(e.errorpos);
							cout.fill('~');
							cout<<'^'<<endl;
							switch(e.errorstatus)
							{
								case exp_parser<type>::error::unexpected:
								cout<<"  syntax error\n";
								break;
								case exp_parser<type>::error::undefined_var:
								cout<<"  undefined variable\n";
								break;
								case exp_parser<type>::error::predefined_const:
								cout<<"  predefined constant\n";
								break;
							}
					}
				}
			}while(!cin.eof());	
		cout<<'\n';
		return 0;
}
