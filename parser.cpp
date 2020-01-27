#include <iostream>
#include <cctype>
#include "lexer.h"
#include "tokens.h"
#include "parser.h"
#include "string.h"
using std::endl;
using std::cout;
using std::string;

static int lookahead, nextTok;
static string lexbuf, nextbuf;

void expressionList();
void identExp();

void error(){
	cout<< "Error!!" <<endl;
}

int peek(){
	if(!nextTok){
	  nextTok = yylex();
	  nextbuf = yytext;
	}
	return nextTok;
}

void match(int tok)
{
  if (lookahead != tok){
    error();
  }
  if(nextTok){
  	lookahead = nextTok;
	lexbuf = nextbuf;
	nextTok = 0;	
  }
  else{
  	lookahead = yylex();
  	lexbuf = yytext;
  }
}

void expression( )
{
    logicalAnd();
    while(lookahead == OR){
        match(OR);
        logicalAnd();
        cout << "or" <<endl;
    }
}

void logicalAnd()
{
    logicalEq();
    while(lookahead == AND )
    {
        match(AND);
        logicalEq();
        cout <<"and"<<endl;
    }
}

void logicalEq()
{
    compExp();
    while(1)
    {
        if(lookahead == EQL){
            match(EQL);
            compExp();
            cout<< "eql"<<endl;
        }else if(lookahead == NEQ ){
            match(NEQ);
            compExp();
            cout<<"neq"<<endl;
        }else{
            break;}
    }
}

void compExp()
{
    addExp();
    while(1)
    {
        if(lookahead == LTN){
            match(LTN);
            addExp();
            cout <<"ltn"<<endl;
        }else if(lookahead == GTN){
            match(GTN);
            addExp();
            cout<<"gtn"<<endl;
        }else if(lookahead == LEQ){
            match(LEQ);
            addExp();
            cout<<"leq"<<endl;
        }else if(lookahead == GEQ){
            match(GEQ);
            addExp();
            cout<<"geq"<<endl;
        }else{
            break;}
    }
}

void addExp()
{
    mulExp();
    while(1)
    {
        if(lookahead == PLUS){
            match(PLUS);
            mulExp();
            cout<<"add"<<endl;
        }else if(lookahead == MINUS){
            match(MINUS);
            mulExp();
            cout<<"sub"<<endl;
        }else{
            break;}
    }
}
void mulExp(){
    preExp();
    while(1)
    {
        if(lookahead == STAR){
            match(STAR);
            preExp();
            cout<<"mul"<<endl;
        }else if(lookahead == DIV){
            match(DIV);
            preExp();
            cout<<"div"<<endl;
        }else if(lookahead == REM){
            match(REM);
            preExp();
            cout<<"rem"<<endl;
        }else{
            break;}
    }
}

void preExp(){
   
        if(lookahead == SIZEOF){
            match(SIZEOF);
            expression();
            cout<<"sizeof"<<endl;
        }else if(lookahead == MINUS){
            match(MINUS);
            preExp();
            cout<<"neg"<<endl;
        }else if(lookahead == NOT){
            match(NOT);
            preExp();
            cout<<"not"<<endl;
        }else if(lookahead == STAR){
            match(STAR);
            preExp();
            cout<<"deref"<<endl;
        }else if(lookahead == ADDR){
            match(ADDR);
            preExp();
            cout<<"addr"<<endl;
        }else{
            postExp();}
}
void postExp(){
    identExp();
    while(1){
        if(lookahead == LBRACK)
        {
            match(LBRACK);
            expression();
            match(RBRACK);
            cout<<"index"<<endl;
        }else if(lookahead == INC){
            match(INC);
            cout<<"inc"<<endl;
        }else if(lookahead == DEC){
            match(DEC);
            cout<<"dec"<<endl;
        }else{
            break;}
    }
}

void identExp(){	
	if(lookahead == ID){
		match(ID);	
		if(lookahead == LPAREN){			
			match(LPAREN);
           	if(lookahead == RPAREN){
            	match(RPAREN);
			}else {		
				expressionList();					
				match(RPAREN);
		     }
		  }					 
    	}
	else if( lookahead == REAL){
		    match(REAL);	
	}
	else if( lookahead == CHARACTER){
	        match(CHARACTER);
	}
	else if( lookahead == STRING){
	        match(STRING);
	}
	else if( lookahead == LPAREN){
	   	match(LPAREN);
        expression();
	    match(RPAREN);
    }
	else if(lookahead == INTEGER){
		match(INTEGER);
	}
}
void expressionList()
{
	expression();
	if(lookahead == ','){
		match(',');
		expressionList();
	}
}
void specifier(){
	if (lookahead == DOUBLE){
		match(DOUBLE);
	}else if(lookahead == CHAR)
	{
		match(CHAR);	
	}else if(lookahead == INT){
		match(INT);
	}

}
int main(void){
    lookahead = yylex();//read first token
    while(lookahead != DONE){
        expression();
        //translationUnit();
    }
	return 0;
}

