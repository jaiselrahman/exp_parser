/**
*
*	Expression Parser
*		- to evaluate simple math expression
*
*	Mathematical expression parser
*	written in c++
*
*	by   jaisel rahman
*		<jaiselrahman@gmail.com>
*
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

namespace EP {

enum class error : char { noerror, undefined_var, unexpected, predefined_const };

enum class type  : char { variable, constant };

template<typename T>
class exp_parser {
  public:

    using f_ptr = T(*)(T);

    exp_parser();

    exp_parser(const std::string &exp);

    bool parse();

    bool parse(const std::string &exp);

    bool addVariable(const std::string &var, const T &val=0,
                     type _type=type::variable);

    bool findVariable(const std::string &var,T &val);

    void addFunction(const std::string &func, const f_ptr &_f_ptr);

    bool findFunction(const std::string &func, f_ptr &_f_ptr);

    bool isConst(const std::string &var);

    T getValue();

    error getErrorStatus();

    size_t getErrorPos();

  private:

    std::string _exp;

    size_t exp_length;

    char look;

    size_t pos;

    T prev_value;

    error errorstatus;

    int errorpos;

    T value;

    std::map<std::string,std::pair<T,type>> v_table;

    std::map<std::string,f_ptr> f_table;

    void eatspace();

    void getlook();

    void seterror(error e);

    bool match(char c);

    std::string getvar();

    T getnum();

    T getvalue();

    bool isvalidop(char c);

    bool isaddop(char c);

    bool ismulop(char c);

    bool isdigit(char c);

    bool isalpha(char c);

    bool isbase(char c, int8_t base);

    T base_to_dec_s(const std::string& base, int8_t b);

    int8_t to_base_int(char c);

    T assignment();

    T expression();

    T term();

    T factor();
};
};

template<typename T>
void EP::exp_parser<T>::eatspace() {
    while(look==' '||look=='\t') {
        getlook();
    }
}

template<typename T>
void EP::exp_parser<T>::getlook() {
    if(pos<=exp_length) {
        look=_exp[pos++];
    } else {
        look=0;
        --pos;
    }
}

template<typename T>
void EP::exp_parser<T>::seterror(error e) {
    errorstatus=e;
    errorpos=pos;
    look=0;
}

template<typename T>
bool EP::exp_parser<T>::match(char c) {
    if(look==c) {
        getlook();
        eatspace();
        return true;
    } else {
        return false;
    }
}

template<typename T>
std::string  EP::exp_parser<T>::getvar() {
    std::string var;
    if(!isalpha(look)) {
        seterror(error::unexpected);
        return 0;
    } else {
        while(isalpha(look)||isdigit(look)) {
            var+=look;
            getlook();
        }
        eatspace();
    }
    return var;
}

template<typename T>
T EP::exp_parser<T>::getnum() {
    T val=0;

    if(!(isdigit(look)||look=='.')) {
        seterror(error::unexpected);
        return 0;
    } else {
        int8_t base=10;
        bool dec_found=false;
        bool exp_found=false;
        if(look=='0') {
            getlook();
            switch(look) {
            case 'x':
            case 'X':
                base=16;
                getlook();
                break;
            case 'b':
            case 'B':
                base=2;
                getlook();
                break;
            default:
                base=8;
                break;
            }
        }
        std::string s;
        while( isbase(look,base) || ( look=='.' && dec_found==false )) {
            s+=look;
            if(look=='.') {
                dec_found=true;
                getlook();
                if(isdigit(look)) {
                    s+=look;
                } else {
                    pos--;
                    break;;
                }
            }
            getlook();
            if(base==10 && (look=='e'||look=='E') && exp_found==false) {
                exp_found=true;
                s+=look;
                getlook();
                if(isaddop(look)||isbase(look,base)) {
                    s+=look;
                    getlook();
                } else {
                    pos--;
                    break;
                }
            }
        }
        eatspace();
        if(base==10)
            return stold(s);

        return base_to_dec_s(s,base);
    }
    return 0;
}

template<typename T>
T  EP::exp_parser<T>::base_to_dec_s(const std::string& base,int8_t b) {
    T d=0;
    bool decimal=false;
    size_t i1=base.find('.'),i2=0;
    i1=(i1==std::string::npos)?base.length():i1;
    for(char c:base) {
        if(c=='.') {
            decimal=true;
            continue;
        }
        if(decimal==false)
            d+=to_base_int(c)*std::pow(b,--i1);
        else
            d+=to_base_int(c)/std::pow(b,++i2);
    }
    return d;
}

template<typename T>
T EP::exp_parser<T>::getvalue() {
    T val=0;
    if(match('a')) {
        val=prev_value;
    } else {
        f_ptr fptr;
        std::string var=getvar();
        if((findFunction(var,fptr))) {
            val=(fptr)(expression());
        } else {
            findVariable(var,val);
        }
    }
    return val;
}

template<typename T>
bool EP::exp_parser<T>::isvalidop(char c ) {
    return ( ismulop(c) || isaddop(c) || c=='(' || c==')' || c=='^' );
}

template<typename T>
bool EP::exp_parser<T>::isaddop(char c) {
    return ( c=='+' || c=='-' );
}

template<typename T>
bool EP::exp_parser<T>::ismulop(char c) {
    return ( c=='*' || c=='/' || c == '(' || c == '%' );
}

template<typename T>
bool EP::exp_parser<T>::isdigit(char c) {
    return ( c>='0' && c<= '9' );
}

template<typename T>
bool EP::exp_parser<T>::isalpha(char c) {
    return ( ( c >= 'a' && c <= 'z' )||( c >='A' && c <= 'Z' ) );
}

template<typename T>
int8_t EP::exp_parser<T>::to_base_int(char c) {
    switch(c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c-'0';
    case 'a':
    case 'A':
        return 10;
    case 'b':
    case 'B':
        return 11;
    case 'c':
    case 'C':
        return 12;
    case 'd':
    case 'D':
        return 13;
    case 'e':
    case 'E':
        return 14;
    case 'f':
    case 'F':
        return 15;
    default:
        return -1;
    }
}

template<typename T>
bool EP::exp_parser<T>::isbase(char c,int8_t base) {
    switch(c) {
    case '0':
    case '1':
        if(base>=2)
            return true;
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
        if(base>=8)
            return true;
    case '8':
    case '9':
        if(base>=10)
            return true;
    case 'A':
    case 'a':
    case 'B':
    case 'b':
    case 'C':
    case 'c':
    case 'D':
    case 'd':
    case 'E':
    case 'e':
    case 'F':
    case 'f':
        if(base==16)
            return true;
    default:
        return false;
    }
    return false;
}

template<typename T>
T EP::exp_parser<T>::assignment() {
    std::string var;
    T val;
    if(isalpha(look)) {
        var=getvar();
        if(match('=')) {
            if(isConst(var)) {
                pos-=2;
                seterror(error::predefined_const);
                return 0;
            }
            val=expression();
            addVariable(var,val);
            return val;
        } else {
            pos-=var.length()+1;
            getlook();
            return expression();
        }
    } else
        return expression();
}

template<typename T>
T EP::exp_parser<T>::expression() {
    T val;
    val=term();
    while(isaddop(look)) {
        switch(look) {
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
T EP::exp_parser<T>::term() {
    T val;
    val=factor();
    while(ismulop(look)) {
        switch(look) {
        case '(':
            //match('(') doesn't need here since factor() takes care of it
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
        case '%':
            match('%');
            val=(int)val%(int)factor();
            break;
        }
    }
    return val;
}

template<typename T>
T EP::exp_parser<T>::factor() {
    T val=0;
    if(match('(')) {
        val=expression();
        if(!match(')')) {
            seterror(error::unexpected);
            return 0;
        } else if(isdigit(look)||look=='.') {
            look='*';
            --pos;
        }
    } else if(isaddop(look)) {
        if(match('-'))
            val=-factor();
        else if(match('+'))
            val=factor();
    } else if(isdigit(look)||look=='.') {
        val=getnum();
    } else if(isalpha(look)) {
        val=getvalue();
    } else {
        seterror(error::unexpected);
        return 0;
    }

    if(match('^')) {
        return pow(val,factor());
    } else if(isalpha(look)) {
        val*=getvalue();
    }

    return val;
}


template<typename T>
EP::exp_parser<T>::exp_parser() {
    prev_value=0;
    pos=0;
    _exp="0";
    errorstatus=error::noerror;
    errorpos=0;
    exp_length=0;
    value=0;
    look='\0';
}

template<typename T>
EP::exp_parser<T>::exp_parser(const std::string &exp) {
    exp_parser();
    exp_length=exp.length();
    _exp=exp;
}

template<typename T>
bool EP::exp_parser<T>::parse() {
    pos=0;
    errorstatus=error::noerror;
    errorpos=-1;
    getlook();
    eatspace();
    value=assignment();
    if( pos<=exp_length && errorstatus==error::noerror ) {
        seterror(error::unexpected);
    }
    if(errorstatus==error::noerror) {
        prev_value=value;
        return true;
    } else
        return false;
}

template<typename T>
bool EP::exp_parser<T>::parse(const std::string &exp) {
    exp_length=exp.length();
    this->_exp=exp;
    return parse();
}

template<typename T>
T EP::exp_parser<T>::getValue() {
    return value;
}

template<typename T>
EP::error EP::exp_parser<T>::getErrorStatus() {
    return errorstatus;
}

template<typename T>
size_t EP::exp_parser<T>::getErrorPos() {
    return errorpos;
}

template<typename T>
bool EP::exp_parser<T>::addVariable(const std::string &var,const T &val,type _type) {
    auto q = v_table.find(var);

    if (q==v_table.end()||q->second.second==type::variable) {
        v_table[var]=std::make_pair(val,_type);
        return true;
    } else if(q->second.second==type::constant) {
        seterror(error::predefined_const);
        return false;
    }
    return false;
}

template<typename T>
bool EP::exp_parser<T>::findVariable(const std::string &var,T &val) {
    auto q = v_table.find(var);
    if (q==v_table.end()) {
        --pos;
        seterror(error::undefined_var);
        return false;
    } else {
        val=q->second.first;
        return true;
    }
}

template<typename T>
bool EP::exp_parser<T>::isConst(const std::string &var) {
    auto q = v_table.find(var);
    if (q->second.second==type::constant) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
void EP::exp_parser<T>::addFunction(const std::string &func,const f_ptr& _f_ptr) {
    f_table[func]=_f_ptr;
}

template<typename T>
bool EP::exp_parser<T>::findFunction(const std::string &func,f_ptr& _f_ptr) {
    auto q=f_table.find(func);
    if (q==f_table.end()) {
        return false;
    } else {
        _f_ptr=q->second;
        return true;
    }
}

#endif //_EXP_PARSER_H_
