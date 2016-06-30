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
		errorstatus=true;
}

bool exp_parser::match(char c) 
{
		#ifndef  NDEBUG
			cout<<"match ="<<c<<"\n";
		#endif
		if(look==c)
		{
				getchar();
				eatspace();
				return true;
		}
		else
		{
			unexpected();
			return false;
		}
}
	
double exp_parser::getnum()
{
		double val=0;
		if(!(isdigit(look)||look=='.'))
				unexpected();
		else
		{
				bool dec_found=false;
				bool exp_found=false;
				stringstream s;
				while(isdigit(look)||(look=='.'&&dec_found==false))//||((look=='E'||look=='e')&&exp_found==true))
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
		
double exp_parser::expression()
{
		double val;
		if(isaddop(look))
				val=0;
		else
		val=term();	
	//	if(!isvalidop(look))
	//			unexpected();
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

double exp_parser::term()
{
		double val;
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

double exp_parser::factor()
{
		double val;
		if(look=='(') 
		{
				match('(');
				val=expression();
				match(')');
				if(isdigit(look))
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
		if(look=='^')
		{
				match('^');
				val=pow(val,factor());
		}
		cout<<"f2 val ="<<val<<"\n";
		if( !look || !isvalidop(look))
				unexpected(); 

		return val;
}

exp_parser::exp_parser() 
{
}

exp_parser::exp_parser(string exp)
{
		exp_length=exp.length();
		this->exp=exp;			
}
		
bool exp_parser::parse()
{
		pos=0;
		errorstatus=false;
		errorpos=0;
		getchar();
		eatspace();
		value=expression();
		if(pos<exp_length)
				unexpected();
		return !errorstatus;
}

bool exp_parser::parse(string exp)
{
		exp_length=exp.length();
		this->exp=exp;
		return parse();
}
