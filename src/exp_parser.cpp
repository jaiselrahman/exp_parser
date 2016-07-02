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

#include<exp_parser.h>
#include<string>
#ifndef  NDEBUG
#include<iostream>
#endif
void exp_parser::eatspace() 
{
		while(look==' '||look=='	')
		{
				getchar();
		}
}

void exp_parser::getchar()
{
		if(pos<=exp_length)
				look=exp[pos++];
		else
				look=0;
}

void exp_parser::unexpected()
{
		errorpos=pos;
		iserror=true;
}

bool exp_parser::match(char c) 
{
		if(look==c)
		{
				#ifndef  NDEBUG
	            cout<<"match ="<<c<<"\n";
			  	#endif
				getchar();
				eatspace();
				return true;
		}
		else
		{
	//		unexpected();
			return false;
		}
}
	
long  double exp_parser::getnum()
{
		long double val=0;
		if(!(isdigit(look)||look=='.'))
				unexpected();
		else
		{
				bool dec_found=false;
				bool exp_found=false;
				stringstream s;
				while(isdigit(look)||(look=='.'&&dec_found==false))
				{
						s<<look;
						if(look=='.')
							dec_found=true;
						getchar();

						if(look=='e'||look=='E')
						{
								exp_found=true;
							   	s<<look;
								getchar();
								if(isaddop(look)||isdigit(look))
								{
										s<<look;
										getchar();
								}
								else
								{
										pos-=2;
										break;
								}
			            }
				}	
				s>>val;

			/*  stringstream s1,s2;	
				s1<<exp.substr(pos-1);
				s1>>val;
				s2<<val;
				pos+=s2.str().length()-1;
				getchar();
				*/
		}
		return val;
}

bool exp_parser::isvalidop(char c )
{
		return (ismulop(c)||isaddop(c)||look=='('||look==')'||look=='^');
}
		
bool exp_parser::isaddop(char c)
{
		return (c=='+'||c=='-');	
}
		
bool exp_parser::ismulop(char c)
{
		return (c=='*'||c=='/'||c=='(');
}

bool exp_parser::isdigit(char c)
{
		return (c>='0'&&c<='9');
}
		
long double exp_parser::expression()
{
		long double val;
		if(isaddop(look))
				val=0;
		else
		val=term();	
		while(isaddop(look))
		{
				switch(look)
				{		
						case '+':
						match('+');
						val+=term();
						break;
						case '-':
						match('-');
						val-=term();		
				}
		}
		return val;
}

long double exp_parser::term()
{
		long double val;
		val=factor();
		while(ismulop(look))
		{
				switch(look)
				{
						case '(':
						val*=factor();
						break;
						case '*':
						match('*');
						val*=factor();
						break;
						case '/':
						match('/');
						val/=factor();
						break;
				}
		}
		//cout<<"look term ="<<look<<"\n";
		return val;
}

long double exp_parser::factor()
{
		long double val;
		if(match('(')) 
		{
				val=expression();
				match(')');
				if(isdigit(look)||look=='.')
				{
					look='*';
					--pos;
				}
		}
		else
		if(isaddop(look))
				val=expression();
		else
		if(isdigit(look)||look=='.')
		{
				val=getnum();
				eatspace();
		}
		else
		if(match('a'))
		{
				val=prev_value;
		}
		else
				unexpected();
		if(match('^'))
	    {
	            val=pow(val,factor());
	    } 

		return val;
}

exp_parser::exp_parser() 
{
		prev_value=0;
}

exp_parser::exp_parser(string exp)
{
		prev_value=0;
		exp_length=exp.length();
		this->exp=exp;			
}
		
bool exp_parser::parse()
{
		pos=0;
		iserror=false;
		errorpos=0;
		getchar();
		eatspace();
		value=expression();

		if(pos<exp_length)
		{
				unexpected();
				pos++;
		}
		if(!iserror)
				prev_value=value;

		return !iserror;
}

bool exp_parser::parse(string exp)
{
		exp_length=exp.length();
		this->exp=exp;
		return parse();
}
