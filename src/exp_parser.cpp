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
		{
				look=exp[pos++];
		}
		else
		{
				look=0;
				--pos;
		}
}

void exp_parser::seterror(error e) 
{
		errorstatus=e;
		errorpos=pos;
		look=0;
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
				return false;
		}
}

std::string  exp_parser::getvar()
{
		string var;
		if(!isalpha(look))
		{
				seterror(error::unexpected);
		}
		else
		{
			while(isalpha(look)||isdigit(look))
			{
					var+=look;
					getchar();
			}
			eatspace();
		}
		return var;
}
	
MAPM exp_parser::getnum()
{
		MAPM val=0;
		if(!(isdigit(look)||look=='.'))
				seterror(error::unexpected);
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
										pos-=1;
										break;
								}

			            }
				}
				val=s.str().c_str();
		/*
				stringstream s1,s2;	
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

bool exp_parser::isalpha(char c)
{
		return ( look>='a' && look<='z' || look>='A' && look<='Z' );
}

MAPM exp_parser::assignment()
{
		string var;
		MAPM val;
		if(isalpha(look))
		{
			var=getvar();
			if(match('='))
			{
					val=expression();
					var_table[var]=val;
					return val;
			}
			else
			{
				pos-=var.length()+1;
				getchar();
				return expression();
			}
		}
		else
				return expression();
}
		
MAPM exp_parser::expression()
{
		MAPM val;

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

MAPM exp_parser::term()
{
		MAPM val;
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

MAPM exp_parser::factor()
{
		MAPM val=0;
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
		if(isalpha(look))
		{
				if(match('a'))
				{
					val=prev_value;
				}
				else
				{
					string var=getvar();

					auto q = var_table.find(var); 
					if (q==var_table.end()) 
					{ 
							seterror(error::undefined_var);
							--pos; 
					} 
					else 
					{
							val=q->second;
					}
					
				}
		}
		else
				seterror(error::unexpected);
		
		if(match('^'))
		{
					return pow(val,factor());       
		}
		
		return val;
}

exp_parser::exp_parser()
{
		prev_value=0;
}

exp_parser::exp_parser(const string &exp)
{
		exp_parser();
		exp_length=exp.length();
		this->exp=exp;			
}
		
bool exp_parser::parse()
{
		pos=0;
		errorstatus=error::noerror;
		errorpos=0;
		getchar();
		eatspace();
		value=assignment();
		if(pos<=exp_length)
		{
				seterror(error::unexpected);
				errorpos=pos;
		}
		if(errorstatus==error::noerror)
		{
				prev_value=value;
				return true;
		}
		else
				return false;
}

bool exp_parser::parse(const string &exp)
{
		exp_length=exp.length();
		this->exp=exp;
		return parse();
}
