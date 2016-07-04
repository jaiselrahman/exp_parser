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

#ifndef _EXP_PARSER_H_

#define _EXP_PARSER_H_

#include<string>
#include<sstream>
#include<cmath>
#include<map>
using namespace std;

//expression parser class

class exp_parser
{
        public:

        enum class error : char { noerror, undefined_var, unexpected};

        error errorstatus;

        int errorpos;              
		
		long double value;

        long double prev_value;

        map<string,long double> var_table;

        exp_parser();

        exp_parser(const string &exp);

        bool parse();

        bool parse(const string &exp);

		private:
		
		string exp;        //contains expression to be parsed
		
		int exp_length;	   //contains length of expression string
		
		char look;		   //contains next char in expression string
		
		int pos;		   //contains present location if look
		
		void eatspace();   //to ignore white space in expression string
		
		void getchar();    //to get the value of look

		void seterror(error e); //to set the error status

		bool match(char c); 
		
		string getvar();
			
		long double getnum();
		
		bool isvalidop(char c);
		
		bool isaddop(char c);
		
		bool ismulop(char c);

		bool isdigit(char c);

		bool isalpha(char c);

		long double assignment();
		
		long double expression();

		long double term();
		
		long double factor();

};

#endif

