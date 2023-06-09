%option noyywrap

%x CHAR_ENTRY
%x CHAR_ESCAPE
%x CHAR_READ
%x CHAR_WRONG_ESCAPE
%x CHAR_ERROR

%x STRING_START
%x STRING_ESCAPE
%x STRING_ERROR
%x STRING_ERROR_ENTER

%x MLCOMENT
%x MLCOMENTSTAR

%{
    #include "scanner.h"
    #include <stdio.h>
    #include <stdlib.h>
%}

LETTER [a-zA-Z]
LETTER_ {LETTER}|_
DIGIT [0-9]
KEYWORD "if"|"for"|"do"|"int"|"float"|"void"|"switch"|"default"|"else"|"while"|"break"|"char"|"double"|"return"|"case"|"continue"
HORIGAP [ \t]
VERTGAP [\n\r]
GAP {HORIGAP}|{VERTGAP}
DECPOINT \.
ESCCHAR [nt\\'afrbv0\"]
SINGQUO [']
DUOQUO \"

DOUBLESLASH "//"
NOTSLASHENTER [^\n\r]
SLASHCOMMENT {DOUBLESLASH}{NOTSLASHENTER}*("\\\n"{NOTSLASHENTER}*)*"\n"

SLASHSTAR "/*"
NOTSTAR [^\*]*
NOTSLASH [^\/]*
VALIDCOMMENT ([\n]*{NOTSTAR}*((\*){NOTSLASH})*)+
MULTILINECOMMENT {SLASHSTAR}{VALIDCOMMENT}"*/"

EXPONENT [eE][-+]?[0-9]+
INTNUM [0-9]+
FRACNUM [0-9]*{DECPOINT}[0-9]+
INTFLOAT {INTNUM}|{FRACNUM}
EXPFLOAT {INTFLOAT}{EXPONENT}

%%


{HORIGAP}+ {}
{VERTGAP} {lineCount++;}

"#include"{HORIGAP}+[^\n]*"\n" {unrecognizedChar("#");lineCount++;}
"#define"{HORIGAP}+[^\n]*"\n" {unrecognizedChar("#");lineCount++;}
 
{SLASHCOMMENT} {singleLineComment(yytext);}

{SLASHSTAR} {BEGIN (MLCOMENT);insertCommentBuffer(yytext);}
<MLCOMENT>[\*] {BEGIN (MLCOMENTSTAR);insertCommentBuffer(yytext);}
<MLCOMENT><<EOF>> {BEGIN(INITIAL); multiLineCommentError();}
<MLCOMENT>{VERTGAP} {insertCommentBuffer(yytext);}
<MLCOMENT>. {insertCommentBuffer(yytext);}

<MLCOMENTSTAR>[/] {BEGIN (INITIAL); multiLineComment(yytext);}
<MLCOMENTSTAR>{VERTGAP} {BEGIN (MLCOMENT);insertCommentBuffer(yytext);}
<MLCOMENTSTAR>. {BEGIN (MLCOMENT);insertCommentBuffer(yytext);}


"++" {tokenizeOperator(INC, yytext);}
"--" {tokenizeOperator(DEC, yytext);}
"<=" {tokenizeOperator(LE, yytext);}
">=" {tokenizeOperator(GE, yytext);}
"==" {tokenizeOperator(EQU,yytext);}
"!=" {tokenizeOperator(NE, yytext);}
"&&" {tokenizeOperator(AND, yytext);}
"||" {tokenizeOperator(OR, yytext);}
"+" {tokenizeOperator(ADD, yytext);}
"-" {tokenizeOperator(MINUS, yytext);}
"*" {tokenizeOperator(MUL, yytext);}
"/" {tokenizeOperator(DIV, yytext);}
"\%" {tokenizeOperator(MOD, yytext);}
"<" {tokenizeOperator(LT, yytext);}
">" {tokenizeOperator(GT, yytext);}
"=" {tokenizeOperator(ASSIGN, yytext);}
"!" {tokenizeOperator(NOT, yytext);}
"(" {tokenizeOperator(LPAREN, yytext);}
")" {tokenizeOperator(RPAREN, yytext);}
"[" {tokenizeOperator(LTHIRD, yytext);}
"]" {tokenizeOperator(RTHIRD, yytext);}
"{" {tokenizeOperator(LCURL, yytext);}
"}" {tokenizeOperator(RCURL, yytext);}
"," {tokenizeOperator(COMMA, yytext);}
";" {tokenizeOperator(SEMICOLON, yytext);}


{KEYWORD} {keywordTokenizer(yytext);}
{LETTER_}({LETTER_}|{DIGIT})* {identifierTokenizer(yytext);}


{EXPFLOAT}{DECPOINT}({LETTER_}|{DIGIT})* {tokenizeConstNumerals(yytext, ILL_FORMAT);}
{EXPFLOAT}{LETTER_}({DIGIT}|{DECPOINT})* {tokenizeConstNumerals(yytext, ILL_FORMAT);}
{EXPFLOAT} {tokenizeConstNumerals(yytext, FLOAT_TYPE);}
{INTNUM}{LETTER_}({LETTER_}|{DIGIT})* {tokenizeConstNumerals(yytext, ILL_FORMAT);}
{FRACNUM}({DECPOINT}[0-9]*)+({LETTER_}|{DIGIT}|{DECPOINT})* {tokenizeConstNumerals(yytext, EXTRA_DECIMAL);}
{EXPFLOAT}({DECPOINT}[0-9]*)+ {tokenizeConstNumerals(yytext, ILL_FORMAT);}
{INTNUM}{DECPOINT} {tokenizeConstNumerals(yytext, ILL_FORMAT);}

{INTNUM} {tokenizeConstNumerals(yytext, INT_TYPE);}
{FRACNUM} {tokenizeConstNumerals(yytext, FLOAT_TYPE);}


{SINGQUO} {BEGIN (CHAR_ENTRY);}

<CHAR_ENTRY>[\\] {BEGIN (CHAR_ESCAPE);}
<CHAR_ENTRY>{VERTGAP} {BEGIN (INITIAL); flushCharBuffer(UNTERMINATED_CHAR);lineCount++;}
<CHAR_ENTRY>[']	{BEGIN (INITIAL); flushCharBuffer(EMPTY_CHAR);}
<CHAR_ENTRY>[^\\] {BEGIN (CHAR_READ);insertCharBuffer(yytext);}

<CHAR_ESCAPE>{ESCCHAR} {BEGIN (CHAR_READ);insertCharBufferEscape(yytext);}
<CHAR_ESCAPE>[^\n']+ {BEGIN (CHAR_ERROR);insertCharWrong(yytext);}

<CHAR_ERROR>[\n] {BEGIN (INITIAL);flushCharBuffer(UNTERMINATED_CHAR);lineCount++;}
<CHAR_ERROR>{SINGQUO} {BEGIN(INITIAL);flushCharBuffer(MULTIWORD_CHAR);}

<CHAR_READ>[^\n']+ {BEGIN (CHAR_ERROR);insertCharWrong(yytext);}
<CHAR_READ>{SINGQUO} {BEGIN (INITIAL);flushCharBuffer(CHAR_TYPE);}
<CHAR_READ>[\n] {BEGIN (INITIAL);flushCharBuffer(UNTERMINATED_CHAR);lineCount++;}
<CHAR_READ><<EOF>> {BEGIN (INITIAL);flushCharBuffer(UNTERMINATED_CHAR);lineCount++;}

{DUOQUO} {BEGIN (STRING_START);}

<STRING_START>[^\\\n\"]* {insertStringBuffer(yytext);}
<STRING_START>[\\] {BEGIN (STRING_ESCAPE);}
<STRING_START>{VERTGAP} {BEGIN (INITIAL);lineCount++;flushStringBuffer(UNTERMINATED_STRING);}
<STRING_START>{DUOQUO} {BEGIN (INITIAL);flushStringBuffer(STRING_TYPE);}
<STRING_START><<EOF>> {BEGIN (INITIAL);lineCount++;flushStringBuffer(UNTERMINATED_STRING);}

<STRING_ESCAPE>{ESCCHAR} {BEGIN (STRING_START);insertEscStringBuffer(yytext);}
<STRING_ESCAPE>{VERTGAP} {BEGIN (STRING_START);lineCount++;insertStringBuffer("\\\n");}
<STRING_ESCAPE><<EOF>> {BEGIN (INITIAL);lineCount++;flushStringBuffer(UNTERMINATED_STRING);}
<STRING_ESCAPE>. {BEGIN(STRING_START);insertStringBuffer(yytext);}

<STRING_ERROR>[^\n\"]* {insertStringBuffer(yytext);}
<STRING_ERROR>{VERTGAP} {BEGIN (INITIAL);lineCount++;flushStringBuffer(UNRECOGNIZED_STRING);}
<STRING_ERROR>{DUOQUO} {BEGIN (INITIAL);flushStringBuffer(UNRECOGNIZED_STRING);}

<STRING_ERROR_ENTER>{VERTGAP} {BEGIN (STRING_ERROR); lineCount++;}
<STRING_ERROR_ENTER>. {BEGIN (STRING_ERROR);flushStringBuffer(UNRECOGNIZED_STRING);}


. {unrecognizedChar(yytext);}

%%

int main(int argc,char *argv[]){
    if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

    logfile.open("1805055_log.txt");
    tokenfile.open("1805055_token.txt");

	yyin= fin;
	yylex();
	fclose(yyin);

	logfile << "\nLine count: " << lineCount << endl;
	logfile << "Error count: " << lexErrorCount << endl;

    logfile.close();
    tokenfile.close();
	return 0;
}