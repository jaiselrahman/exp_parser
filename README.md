# EXP_PARSER 
------------
It is a *c++ template class* used to evaluate simple mathematical expression in string

#### Supports basic mathematical operators  like

#### ^  power
#### *  multiplication
#### /  division
#### +  addition
#### -  subtraction

#### listed in order of precedence

__( )__ can be used used to group expression and has the highest order of precedence

#### Supports implicit multiplication operator
i.e: '\*' can be omitted in expressions like a\*(b) or (a)\*b

#### Support variables
- Variable name should start with alphabet and can contain numbers

- Variables can be declared by the statement __variable=expression__ in the expression string

- Predefined variable 'a' containes value of previously parsed expressioned

exp_parser<T> class is written inside the namespace EP

To use expression parser in your program just include the header file the header file ***'exp_parser.h'*** in your program

create an object of  ***EP::exp_parser<*T*>***
***T*** can  be any type that should supported atleast
___+ , - , / , * , >>___ operators

####exp_parser contains following constructor
-	 ***exp_parser()***  -   default constructor
-    ***exp_parser(std::string)***  -  parametrized constructor used to inialize expression string

####exp_parser contains following public member functions
-	 ***bool parser()*** - evaluate the string initialized with ***exp_parser(std::string)*** constructor and returns true if success else false
-    ***bool parser(std::string expression)*** - evaluate the string and return returns true if success else false
-    ***bool add_var(string var_name,T val=0,EP::type _type=EP::type::constant)*** - used to add variable or constant with value of type T
-    ***bool find_var(std::string var,T& val)*** - returns true if var is defined and stores its value in val else false
-    ***bool isconst(std::string var)*** - returns true if var is a constant else false

####exp_parser contains following public data member
-	 ***T value*** - returns the result of last evaluated expression
-    ***enum class EP::error errorstatus*** - All the above public members set this errorstatus
     which takes the values { noerror, undefined_var, unexpected, predefined_const }
-    ***errorpos*** - Contains the position where the error is


####Eg of simple expression parser

```c++   
    #include <iostream.h>
    #include <cmath> 
	#include <exp_parser.h>  /* expression parser header file */

	using namespace std;
	using namespace EP;
    int main()
    {
        exp_parse<double> e; /* create an  expression parser of type double */
        e.add_var("pi",3.14,type::cons);
		e.add_func("log",&log10);
		string s;
        cout << "Enter a mathematical expression : \n ";
        cin >> s;
        e.parser( s );
        cout << "Result : " << e.value;
        return 0;
  	}
```

//TODO: to add support for hex bin oct
