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

#define PI "3.141592653589793238462643383279502884197169399\
375105820974944592307816406286208998628034825342117067982148\
086513282306647093844609550582231725359408128481117450284102\
701938521105559644622948954930381964428810975665933446128475\
648233786783165271201909145648566923460348610454326648213393\
607260249141273724586997472482236150282340795515112055881168\
465696731309335738719301105597412739780116660082344736784152\
495003734848979554541645390198611757272273187138842264358897\
421202171319495680514230839931356624755337162012934002605160\
185668467703312242818785547936550870272311014345824073680634\
179896333892328646035108977272081791959967513336311014750579\
717366267579547177770281431880438556092967247917735054925101\
853767400612361479011038319250289792336799378361931016667901\
318796931517257943860403036395703382632593537215128964016797\
694845390461961548136833293693702683188836758023996908893269\
752781165328222495041033657338594419051644614642369403738060\
905908822203694572794411694624061668484893417030434648040682\
0774078369140625"

using namespace mpfr;
using namespace std;
using namespace EP;
typedef  mpreal d_type;

string trim(const string &t)
{
		int i;
		for(i=0;t[i]==' '||t[i]=='\t';i++);
		return t.substr(i);
}

int main(int argc,char *argv[])
{
		string exp;
		exp_parser<d_type> e;
		e.add_var("pi",mpfr::const_pi(2048),EP::type::constant);
		int n=6;
		mpreal::set_default_prec(mpfr::digits2bits(n));
		if(argc>1)
		{
			bool cl=false;
			int i=1;
				while(argv[i])
				{
					if(string("--help").compare(argv[i])==0)
					{
							cout<<HELP_TEXT;
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
						cout<<HELP_TEXT<<endl;
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
								case EP::error::unexpected:
								cout<<"  syntax error\n";
								break;
								case EP::error::undefined_var:
								cout<<"  undefined variable\n";
								break;
								case EP::error::predefined_const:
								cout<<"  predefined constant\n";
								break;
							}
					}
				}
			}while(!cin.eof());	
		cout<<'\n';
		return 0;
}
