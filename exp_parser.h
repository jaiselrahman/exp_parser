/*
*Exp_parser by jaisel rahman <jaisel20@gmail.com>
*/

#ifndef _EXP_PARSER_H_

#define _EXP_PARSER_H_

#include<string>
#include<sstream>
#include<map>

using namespace std;

namespace EP
{

	//to define the error while parsing
	enum class error : char { noerror, undefined_var, unexpected, predefined_const};

	//type of variable used in add_var()
	enum class type  : char { var, cons };

	
	//exp_parser template class
	template<typename T>
	class exp_parser
	{
	public:
			
			typedef T (*f_ptr)(T);
			
			//contains the value of enum class error set by the parsing functions
			error errorstatus;

        	//contains the index of char which causes the error
			int errorpos;              
		
			//result of the expression
			T value;

			//default constructor
			exp_parser();

        	//constructor initialized with the expression string
			exp_parser(const std::string &exp);

        	//parse the expression string initialized with
			//and return true if success else sets the errorstatus and returns false
			bool parse();

			//parser the arguments and returns true if success else sets errorstatus and returns false
			bool parse(const std::string &exp);

			//adds variable or constant so that it can be used in expression string
			//return true if success else sets the errorstatus and returns false
			bool add_var(const std::string &var,const T &val=0,type _type=type::var);

			//returns true if the `var` is already set else returns false
			bool find_var(const std::string &var,T &val);

			//
			void add_func(const std::string &func,f_ptr _f_ptr);

			//
			bool find_func(const string &func,f_ptr &_f_ptr);
			
			//return true if `var` is constant else  returns false
			bool isconst(const std::string &var);


	private:
		
			string _exp;        //contains expression to be parsed
		
			int exp_length;	   //contains length of expression string
		
			char look;		   //contains next char in expression string
		
			int pos;	//contains present location of `look`
		
			T prev_value;
			
			map<string,pair<T,type>> v_table; //stores the variable name its type and value

			map<string,f_ptr> f_table;

			void eatspace();
		
			void getchar();    //to get the next char and store it in `look`

			void seterror(error e); //to set the error status

			bool match(char c); 
		
			string getvar(); //to get the current variable name
			
			T getnum(); //to get the current name
		
			bool isvalidop(char c);	//if c is any valid operator
		
			bool isaddop(char c);	//if + , -
		
			bool ismulop(char c);	//if *, /

			bool isdigit(char c);	//if c is a number

			bool isalpha(char c);	//if c is a char

			T assignment();			//to handle assignment statement var=val
		
			T expression();			// n1 (+|-) n2

			T term();				// n1 (*|/) n2
		
			T factor();				// n1 (expression|factor)

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
				look=_exp[pos++];
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
		return ( (look>='a' && look<='z')||( look>='A' && look<='Z') );
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
					if(match('('))
					{
							f_ptr fptr;
							if((find_func(var,fptr)))
							{
									val=(fptr)(expression());
									match(')');
							}
					}
					else
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
		this->_exp=exp;
		return parse();
}

template<typename T>
bool EP::exp_parser<T>::add_var(const std::string &var,const T &val,type _type)
{
		auto q = v_table.find(var);

		if (q==v_table.end()||q->second.second==type::var)
		{
				v_table[var]=make_pair(val,_type);
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
		auto q = v_table.find(var);
		if (q==v_table.end())
	    {       
				--pos;
				return false;
				seterror(error::undefined_var);
		}
		else
		{
		        val=q->second.first;
				return true;
		}
}

template<typename T>
bool EP::exp_parser<T>::find_func(const std::string &func,f_ptr& _f_ptr)
{
		auto q=f_table.find(func);
		if (q==f_table.end())
		{
				--pos;
				return false;
		}
		else
		{
				_f_ptr=q->second;
				return true;
		}

}

template<typename T>
void EP::exp_parser<T>::add_func(const std::string &func,f_ptr _f_ptr)
{
		f_table[func]=_f_ptr;
}

template<typename T>
bool EP::exp_parser<T>::isconst(const std::string &var)
{
		auto q = v_table.find(var);
		if (q->second.second==type::cons)
		{
				return true;
		}
		else
		{
				return false;
		}
}

#endif //_EXP_PARSER_H_
