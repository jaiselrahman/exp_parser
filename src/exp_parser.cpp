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
				look=0;
}

void exp_parser::unexpected()
{
		errorpos=pos;
		errorstatus=true;
}

void exp_parser::match(char c) 
{
		#ifndef  NDEBUG
			cout<<"match ="<<c<<"\n";
		#endif
		if(look==c)
		{
				getchar();
				eatspace();
		}
		else
			unexpected();
}
	
int exp_parser::getnum()
{
		int val=0;
		if(!isdigit(look))
				unexpected();
		else
		{
				while(isdigit(look))
				{
						val=val*10+int(look)-int('0');
						getchar();
				}
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
		
float exp_parser::expression()
{
		float val;
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

float exp_parser::term()
{
		float val;
		val=factor();
		while(ismulop(look))
		{
				switch(look)
				{
						case '(':
					//	match('(');
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
				//		case ')':
				//		match(')');
				//		val*=factor();
				//		break;
				}
		}
		//cout<<"look term ="<<look<<"\n";
		return val;
}

float exp_parser::factor()
{
		float val;
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
		if(isdigit(look))
				val=getnum();
		
		if(look=='^')
		{
				match('^');
				val=pow(val,factor());
		}
		//cout<<"f2 look ="<<look<<"\n";
		if( look && !isvalidop(look))
				unexpected();
		eatspace(); 

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
