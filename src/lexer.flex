%option noyywrap

%{
  // A lot of this lexer is based off the ANSI C grammar:
  // https://www.lysator.liu.se/c/ANSI-C-grammar-l.html#MUL-ASSIGN
  // Avoid error "error: `fileno' was not declared in this scope"
  extern "C" int fileno(FILE *stream);
  #include <map>;
  #include <string>;
  #include "parser.tab.hpp"

  std:: map <std:: string, std:: string> TypeDef;

  std::string resolveTypedef(const std::string& id, std::set<std::string>& visited) {
    //real type
    if (!TypeDef.count(id)) return id;

    // cycle
    if (visited.find(id) != visited.end()) {
        std::cerr << "Error: Typedef cycle detected for '" << id << "'." << std::endl;
        exit(1);
    }

    visited.insert(id); // Mark this id as visited to detect cycles.
    return resolveTypedef(TypeDef[id], visited); //next typedef
  }

  // wrapper function
  std::string resolveTypedef(const std::string& id) {
      std::set<std::string> visited;
      return resolveTypedef(id, visited);
  }

  void update_map(std:: string key, std:: string target){
      TypeDef[key] = target;
  }

  auto check_map(std:: string id){

    if (TypeDef.count(id)){
      id = resolveTypedef(id);
      if (id == "int") {
        return(INT);
      }
      else if (id == "float") {
        return(FLOAT);
      }
      else if (id == "char") {
        return(CHAR);
      }
      else if (id == "void") {
        return(VOID);
      }
      else if (id == "double") {
        return(DOUBLE);
      }
      else{
        std::cerr<< "bad type" << std::endl;
      }
    }
    return IDENTIFIER;
  }

%}

D	  [0-9]
L	  [a-zA-Z_]
H   [a-fA-F0-9]
E	  [Ee][+-]?{D}+
FS  (f|F|l|L)
IS  (u|U|l|L)*

%%
"/*"			{/* consumes comment - TODO you might want to process and emit it in your assembly for debugging */}

"auto"			{return(AUTO);}
"break"			{return(BREAK);}
"case"			{return(CASE);}
"char"			{return(CHAR);}
"const"			{return(CONST);}
"continue"  {return(CONTINUE);}
"default"		{return(DEFAULT);}
"do"			  {return(DO);}
"double"		{return(DOUBLE);}
"else"			{return(ELSE);}
"enum"			{return(ENUM);}
"extern"		{return(EXTERN);}
"float"			{return(FLOAT);}
"for"			  {return(FOR);}
"goto"			{return(GOTO);}
"if"			  {return(IF);}
"int"			  {return(INT);}
"long"			{return(LONG);}
"register"	{return(REGISTER);}
"return"		{return(RETURN);}
"short"			{return(SHORT);}
"signed"		{return(SIGNED);}
"sizeof"		{return(SIZEOF);}
"static"		{return(STATIC);}
"struct"		{return(STRUCT);}
"switch"		{return(SWITCH);}
"typedef"		{return(TYPEDEF);}
"union"			{return(UNION);}
"unsigned"	{return(UNSIGNED);}
"void"			{return(VOID);}
"volatile"	{return(VOLATILE);}
"while"			{return(WHILE);}

{L}({L}|{D})*		{yylval.string = new std::string(yytext); return (check_map(std::string(yytext)));}

0[xX]{H}+{IS}?		{yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}
0{D}+{IS}?		    {yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}
{D}+{IS}?		      {yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}

{D}+{E}{FS}?[fF]            { yylval.number_float = strtof(yytext, NULL); return (FLOAT_CONSTANT); }
{D}*"."{D}+({E})?{FS}?[fF]  { yylval.number_float = strtof(yytext, NULL); return (FLOAT_CONSTANT); }
{D}+"."{D}*({E})?{FS}?[fF]  { yylval.number_float = strtof(yytext, NULL); return (FLOAT_CONSTANT); }
{D}+{E}{FS}?                { yylval.number_float = strtod(yytext, NULL); return (DOUBLE_CONSTANT); }
{D}*"."{D}+({E})?{FS}?      { yylval.number_float = strtod(yytext, NULL); return (DOUBLE_CONSTANT); }
{D}+"."{D}*({E})?{FS}?      { yylval.number_float = strtod(yytext, NULL); return (DOUBLE_CONSTANT); }


L?'(\\.|[^\\'])+'			{ yylval.string=new std::string(yytext); return (CHAR_LITERAL); }
L?\"(\\.|[^\\"])*\"	{yylval.string=new std::string(yytext); return(STRING_LITERAL);}

"..."      {return(ELLIPSIS);}
">>="			 {return(RIGHT_ASSIGN);}
"<<="      {return(LEFT_ASSIGN);}
"+="			 {return(ADD_ASSIGN);}
"-="       {return(SUB_ASSIGN);}
"*="       {return(MUL_ASSIGN);}
"/="			 {return(DIV_ASSIGN);}
"%="			 {return(MOD_ASSIGN);}
"&="       {return(AND_ASSIGN);}
"^="			 {return(XOR_ASSIGN);}
"|="       {return(OR_ASSIGN);}
">>"       {return(RIGHT_OP);}
"<<"       {return(LEFT_OP);}
"++"			 {return(INC_OP);}
"--"			 {return(DEC_OP);}
"->"			 {return(PTR_OP);}
"&&"			 {return(AND_OP);}
"||"			 {return(OR_OP);}
"<="			 {return(LE_OP);}
">="			 {return(GE_OP);}
"=="			 {return(EQ_OP);}
"!="			 {return(NE_OP);}
";"			   {return(';');}
("{"|"<%") {return('{');}
("}"|"%>") {return('}');}
","			   {return(',');}
":"			   {return(':');}
"="			   {return('=');}
"("		     {return('(');}
")"			   {return(')');}
("["|"<:") {return('[');}
("]"|":>") {return(']');}
"."			   {return('.');}
"&"			   {return('&');}
"!"			   {return('!');}
"~"			   {return('~');}
"-"			   {return('-');}
"+"			   {return('+');}
"*"			   {return('*');}
"/"			   {return('/');}
"%"			   {return('%');}
"<"			   {return('<');}
">"			   {return('>');}
"^"			   {return('^');}
"|"			   {return('|');}
"?"			   {return('?');}

[ \a\b\t\v\f\n\r]		{/* ignore new lines and special sequences */}
.			              {/* ignore bad characters */}

%%

void yyerror (char const *s)
{
  fprintf(stderr, "Lexing error: %s\n", s);
  exit(1);
}
