/**
*
*	Expression Parser
*		- to evaluate simple math expression
*
*	written in c++
*
*	by   jaisel rahman
*		<jaisel20@gmail.com>
*
*	Exp version by jaisel rahman <jaisel20@gmail.com>
*	Mathemetical expression parser
*
*	It is licensed under LGPLV3
*	See LICENSE file
*
*	For more information read README file
**/

#ifndef _EXP_PARSER_H_
#define _EXP_PARSER_H_
#include<string>
#include<sstream>
#include<map>

namespace EP
{

//to define the error while parsing
enum class error : char { noerror, undefined_var, unexpected, predefined_const };

//type of variable used in add_var()
enum class type  : char { variable , constant };

//exp_parser template class
    template<typename T>
    class exp_parser
    {
    public:

        //poiter to a function
        typedef T (*f_ptr)(T);

        //Contains the value of enum class error set by the parsing functions
        error errorstatus;

        //Contains the index of char which causes the error
        int errorpos;

        //Result of the expression
        T value;

        //Default constructor
        exp_parser();

        //Constructor initialized with the string to be parsed
        exp_parser(const std::string &exp);

        //parse the expression string and
        //return true if success else sets the errorstatus and returns false
        bool parse();

        //parser the arguments and returns true if success else sets errorstatus and returns false
        bool parse(const std::string &exp);

        //adds variable or constant to `v_table` so that it can be used in expression string
        //return true if success else sets the errorstatus and returns false
        bool add_var(const std::string &var,const T &val=0,type _type=type::variable);

        //returns true if the `var` is already set else returns false
        bool find_var(const std::string &var,T &val);

        //adds function in `f_table`
        //return true if success else sets the errorstatus and returns false
        void add_func(const std::string &func,f_ptr _f_ptr);

        ////returns true if the `var` is already set else returns false
        bool find_func(const std::string &func,f_ptr &_f_ptr);

        //return true if `var` is constant else  returns false
        bool isconst(const std::string &var);


    private:

        std::string _exp;       //contains expression to be parsed

        uint64_t exp_length;    //contains length of expression string

        char look;              //contains next char in expression string

        uint64_t pos;	        //contains present location of `look`

        T prev_value;           //contains the value of previously executed value

        std::map<std::string,std::pair<T,type>> v_table; //stores the variable name, its type and value

        std::map<std::string,f_ptr> f_table;     //stores th function name and pointere to the function

        void eatspace();        //to ignore whitespaces and tabs

        void getchar();         //to get the next char and store it in `look`

        void seterror(error e); //to set the errorstatus

        bool match(char c);     // if look == c

        std::string getvar();   //to get the current variable name

        T getnum();             //to get the current name

        T getvalue();           //to get value from variable, function or previous value

        bool isvalidop(char c);	//if c is any valid operator

        bool isaddop(char c);	//if + , -

        bool ismulop(char c);	//if *, /

        bool isdigit(char c);	//if c is a number

        bool isalpha(char c);	//if c is a char

        T assignment();			// to handle assignment statement var=val

        T expression();			// n1 (+|-) n2

        T term();				// n1 (*|/) n2

        T factor();				// n1 (expression|factor)

    };

};

template<typename T>
void EP::exp_parser<T>::eatspace()
{
    while(look==' '||look=='\t')
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
    std::string var;
    if(!isalpha(look))
    {
        seterror(error::unexpected);
        return 0;
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
    {
        seterror(error::unexpected);
        return 0;
    }
    else
    {
        bool dec_found=false;
        bool exp_found=false;
        std::stringstream s;
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
                    pos--;
                    break;
                }
            }
        }
        s>>val;
        eatspace();
    }
    return val;
}

template<typename T>
T EP::exp_parser<T>::getvalue()
{
    T val=0;
    if(match('a'))
    {
        val=prev_value;
    }
    else
    {
        f_ptr fptr;
        std::string var=getvar();
        if((find_func(var,fptr)))
        {
            val=(fptr)(expression());
        }
        else
        {
            find_var(var,val);
        }
    }
    return val;
}

template<typename T>
bool EP::exp_parser<T>::isvalidop(char c )
{
    return (ismulop(c)||isaddop(c)||c=='('||c==')'||c=='^');
}

template<typename T>
bool EP::exp_parser<T>::isaddop(char c)
{
    return ( c=='+' || c=='-' );
}

template<typename T>
bool EP::exp_parser<T>::ismulop(char c)
{
    return ( c=='*' || c=='/' || c == '(' );
}

template<typename T>
bool EP::exp_parser<T>::isdigit(char c)
{
    return ( c>='0' && c<= '9' );
}

template<typename T>
bool EP::exp_parser<T>::isalpha(char c)
{
    return ( ( c >= 'a' && c <= 'z' )||( c >='A' && c <= 'Z' ) );
}

template<typename T>
T EP::exp_parser<T>::assignment()
{
    std::string var;
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
        if(!match(')'))
        {
            seterror(error::unexpected);
            return 0;
        }
        if(isdigit(look)||look=='.')
        {
            look='*';
            --pos;
        }
    }
    else if(isaddop(look))
    {
        if(match('-'))
            val=-factor();
        else if(match('+'))
            val=factor();
    }
    else if(isdigit(look)||look=='.')
    {
        val=getnum();
    }
    else if(isalpha(look))
    {
        val=getvalue();
    }
    else
    {
        seterror(error::unexpected);
        return 0;
    }

    if(match('^'))
    {
        return pow(val,factor());
    }
    if(isalpha(look))
    {
        val*=getvalue();
    }

    return val;
}


template<typename T>
EP::exp_parser<T>::exp_parser()
{
    prev_value=0;
}

template<typename T>
EP::exp_parser<T>::exp_parser(const std::string &exp)
{
    this->exp_parser();
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
    if( pos<=exp_length && errorstatus==error::noerror )
    {
        seterror(error::unexpected);
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
bool EP::exp_parser<T>::parse(const std::string &exp)
{
    exp_length=exp.length();
    this->_exp=exp;
    return this->parse();
}

template<typename T>
bool EP::exp_parser<T>::add_var(const std::string &var,const T &val,type _type)
{
    auto q = v_table.find(var);

    if (q==v_table.end()||q->second.second==type::variable)
    {
        v_table[var]=std::make_pair(val,_type);
        return true;
    }
    else if(q->second.second==type::constant)
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
    auto q = v_table.find(var);
    if (q->second.second==type::constant)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
void EP::exp_parser<T>::add_func(const std::string &func,f_ptr _f_ptr)
{
    f_table[func]=_f_ptr;
}

template<typename T>
bool EP::exp_parser<T>::find_func(const std::string &func,f_ptr& _f_ptr)
{
    auto q=f_table.find(func);
    if (q==f_table.end())
    {
        return false;
    }
    else
    {
        _f_ptr=q->second;
        return true;
    }
}

#endif //_EXP_PARSER_H_
