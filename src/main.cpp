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
		int n=6;
		if(argc>1)
		{
			stringstream s;
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
											s<<&argv[i][2];
											s>>n;
											m_apm_cpp_precision(n-1);
									}
									break;
									
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
					else
					if(e.parse(argv[i]))
					{
							char c[1024];
							e.value.toFixPtString(c,ALL_DIGITS);
						cout<<" = "<<c<<"\n";
					}
					else
					{
						cerr<<" syntax error";
						return 1;
					}
					i++;
				}
		}
		{
			m_apm_cpp_precision(n-1);
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
							char c[1024];
							e.value.toFixPtString(c,ALL_DIGITS);
							cout<<" = "<<c<<endl;
					}		
					else
					{
							switch(e.errorstatus)
							{
								case exp_parser::error::unexpected:
								cout<<"   ";
								cout.width(e.errorpos);
								cout.fill('~');
								cout<<'^'<<endl;
								cout<<"  syntax error\n";
								break;
								case exp_parser::error::undefined_var:
								cout<<"   ";
								cout.width(e.errorpos);
								cout.fill('~');
								cout<<'^'<<endl;
								cout<<"  undefined variable\n";
								break;
							}
					}
				}
			}while(!cin.eof());	
		cout<<'\n';
		}
		return 0;
}


