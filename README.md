# EXP_PARSER 
==================

It is a * c++ template class * used to evaluate simple mathematical expression in string

      * Supports basic mathematical operators * like

      ** ^  power **
      ** *  multiplication **
      **  /   division **
      ** +  addition **
      **  -  subtraction **

     *  listed in order of precedence *

        **( )** can be used used to group expression and has the highest order of precedence

    *    Supports implicit multiplication operator *
  >	  i.e: '*' can be omitted in expressions like a*(b) or (a)*b

     *  Support variables *
    *  variable name should start with alphabet and can contain numbers *

        Variables can be declared by the statement *variable=expression* in the expression string

        Value of previous expression can be obtained using pre-defined variable '***a***'


To use expression parser in your program just include the header file the header file *'exp_parser.h'* in your program

create an object of  **exp_parser<*T*>  **
***T*** can  be any type that should supported atleast
**  +	 , -  , / , * ** operators

exp_parser contains following constructor
  -	  ***exp_parser()***    -   default constructor
  -	  ***exp_parser(std::string)***  -  parametrized constructor used to inialize expression string

exp_parser contains following public member functions
  -	 ***parser()*** - evaluate the string initialized with exp_parser(std::string) constructor and returns true if success else false
  -	 ***parser(std::string)*** -  evaluate the string and return returns true if success else false

exp_parser contains following public data member
  -	 ***value*** - return the result of last evaluated expression



Eg of simple expression parser

  >	  #include <iostream.h>
  >	  #include <exp_parser.h> /* expression parser header file */
  >	  using namespace std;
  >	  int main()
  >	  {
  >	       exp_parser<double> e;        /* create an  expression parser of type double */
  >	       string s;
  >	       cout << "Enter a mathematical expression : \n ";
  >	       cin >> s;
  >	       e.parser( s );
  >	       cout << "Result : " << e.value;
  >	       return 0;
  >	  }