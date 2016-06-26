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
#include<iostream>
#include<cmath>
using namespace std;

//expression parser class

class exp_parser
{
		private:
		
		string exp;        //contains expression to be parsed
		
		int exp_length;	   //contains length of expression string
		
		char look;		   //contains next char in expression string
		
		int pos;		   //contains present location if look
		
		bool errorstatus;  //whether expression parsing succeeds
		
		void eatspace();   //to ignore white space in expression string
		
		void getchar();    //to get the value of look

		void unexpected(); //to set the error status

		void match(char c); 
	
		int getnum();
		
		bool isvalidop(char c);
		
		bool isaddop(char c);
		
		bool ismulop(char c);

		bool isdigit(char c);
		
		float expression();

		float term();
		
		float factor();

		public:

		int errorpos;
		
		float value;
		
		exp_parser(); 

		exp_parser(string exp);
		
		bool parse();

		bool parse(string exp);
};

#endif

