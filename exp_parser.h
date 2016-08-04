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
*	This header file is licensed under GNU LGPL v3
*
**/

#ifndef _EXP_PARSER_H_

#define _EXP_PARSER_H_

#include<string>
#include<sstream>
#include<map>
using namespace std;

//expression parser class
namespace EP
{
	enum class error : char { noerror, undefined_var, unexpected, predefined_const};

	enum class type  : char { var, cons };

	template<typename T>
	class exp_parser
	{
	public:
			
			error errorstatus;

        	int errorpos;              
		
			T value;

			exp_parser();

        	exp_parser(const std::string &exp);

        	bool parse();
        
			bool parse(const std::string &exp);

			bool add_var(const std::string &var,const T &val=0,type _type=type::var);

			bool find_var(const std::string &var,T &val);

			bool isconst(const std::string &var);


	private:
		
			string exp;        //contains expression to be parsed
		
			int exp_length;	   //contains length of expression string
		
			char look;		   //contains next char in expression string
		
			int pos;		   //contains present location if look
		
			T prev_value;
			
			map<string,pair<T,type>> var_table;

			void eatspace();   //to ignore white space in expression string
		
			void getchar();    //to get the value of look

			void seterror(error e); //to set the error status

			bool match(char c); 
		
			string getvar();
			
			T getnum();
		
			bool isvalidop(char c);
		
			bool isaddop(char c);
		
			bool ismulop(char c);

			bool isdigit(char c);

			bool isalpha(char c);

			T assignment();
		
			T expression();

			T term();
		
			T factor();

};

};

template<typename T>
void EP::exp_parser<T>::eatspace() 
{
		while(look==' '||look=='	')
		{
				getchar();
		}
}

template<typename T>
void EP::exp_parser<T>::getchar()
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

template<typename T>
void EP::exp_parser<T>::seterror(error e) 
{
		errorstatus=e;
		errorpos=pos;
		pos=exp_length;
		look=0;
}

template<typename T>
bool EP::exp_parser<T>::match(char c) 
{
		if(look==c)
		{
				getchar();
				eatspace();
				return true;
		}
		else
		{
				return false;
		}
}

template<typename T>
std::string  EP::exp_parser<T>::getvar()
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
	
template<typename T>
T EP::exp_parser<T>::getnum()
{
		T val=0;
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
						{
								dec_found=true;
								getchar();
								if(isdigit(look))
								{
										s<<look;
								}
								else
								{
									 pos--;
									 break;;
								}
						}
						
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
				s>>val;
		}
		return val;
}

template<typename T>
bool EP::exp_parser<T>::isvalidop(char c )
{
		return (ismulop(c)||isaddop(c)||look=='('||look==')'||look=='^');
}

template<typename T>
bool EP::exp_parser<T>::isaddop(char c)
{
		return (c=='+'||c=='-');
}

template<typename T>
bool EP::exp_parser<T>::ismulop(char c)
{
		return (c=='*'||c=='/'||c=='(');
}

template<typename T>
bool EP::exp_parser<T>::isdigit(char c)
{
		return (c>='0'&&c<='9');
}

template<typename T>
bool EP::exp_parser<T>::isalpha(char c)
{
		return ( look>='a' && look<='z' || look>='A' && look<='Z' );
}

template<typename T>
T EP::exp_parser<T>::assignment()
{
		string var;
		T val;
		if(isalpha(look))
		{
			var=getvar();
			
			if(match('='))
			{
					if(isconst(var))
					{
							pos-=2;
							seterror(error::predefined_const);
							return 0;
					}
					val=expression();
					add_var(var,val);
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
		
template<typename T>
T EP::exp_parser<T>::expression()
{
		T val;
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

template<typename T>
T EP::exp_parser<T>::term()
{
		T val;
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
		return val;
}

template<typename T>
T EP::exp_parser<T>::factor()
{
		T val=0;
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
		{
				if(match('-'))
						val=-factor();
				else 
				if(match('+'))
						val=factor();
		}
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
					find_var(var,val);
				}
		}
		else
		{
				seterror(error::unexpected);
		}
		
		if(match('^'))
		{
				return pow(val,factor());       
		}
		if(isalpha(look))
		{
				if(match('a'))
				{
						val=prev_value;
				}
				else
				{
						T tmp;
						string var=getvar();
						find_var(var,tmp);
						val*=tmp;
				}
		}
		return val;
}



template<typename T>
EP::exp_parser<T>::exp_parser()
{
		prev_value=0;
}


template<typename T>
EP::exp_parser<T>::exp_parser(std::string const &exp)
{
		exp_parser();
		exp_length=exp.length();
		this->exp=exp;			
}
		
template<typename T>
bool EP::exp_parser<T>::parse()
{
		pos=0;
		errorstatus=error::noerror;
		errorpos=-1;
		getchar();
		eatspace();
		value=assignment();
		if(pos<exp_length)
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

template<typename T>
bool EP::exp_parser<T>::parse(std::string const &exp)
{
		exp_length=exp.length();
		this->exp=exp;
		return parse();
}

template<typename T>
bool EP::exp_parser<T>::add_var(const std::string &var,const T &val,type _type)
{
		auto q = var_table.find(var);

		if (q==var_table.end()||q->second.second==type::var)
		{
				var_table[var]=make_pair(val,_type);
				return true;
		}
		else
		if(q->second.second==type::cons)
		{
		       seterror(error::predefined_const);
			   return false;
		}
		return false;
}

template<typename T>
bool EP::exp_parser<T>::find_var(const std::string &var,T &val)
{
		auto q = var_table.find(var);
		if (q==var_table.end())
	    {       
				--pos;
				seterror(error::undefined_var);
				return false;
		}
		else
		{
		        val=q->second.first;
				return true;
		}
}

template<typename T>
bool EP::exp_parser<T>::isconst(const std::string &var)
{
		auto q = var_table.find(var);
		
		if (q->second.second==type::cons)
		{
				return true;
		}
		else
		{
				return false;
		}
}
#endif	//_EXP_PARSER_H_
