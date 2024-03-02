// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

// TODO: you can either continue adding to this grammar file or
// rename parser_full.y to parser.y once you're happy with
// how this example works.

%code requires{
    #include "ast.hpp"

    extern Node *g_root;
    extern FILE *yyin;
    int yylex(void);
    void yyerror(const char *);
}

// Represents the value associated with any kind of AST node.
%union{
  Node         *node;
  NodeList     *nodes;
  int          number_int;
  double       number_float;
  std::string  *string;
  yytokentype  token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> TypeSpecifier  function
%type <node> external_declaration compound_statement
%type <node> statement  expr
%type <node> jump_statement BASE

%type <nodes> functionsGroup declaration statement_list

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER


%start ROOT

%%
// returns the root
ROOT
: functionsGroup {g_root = $1;}
;
//group of function and global variables
functionsGroup
: external_declaration	{$$ = new NodeList($1);}
| functionsGroup external_declaration {$1 -> PushBack($1); $$ = $1;}
;
//global variable is declared here
external_declaration
: function {$$ = $1;}
| declaration {$$ = $1;}
;
// implementing the different types of function
function
: TypeSpecifier IDENTIFIER '(' ')' ';'  {$$ = new Function($1, *$2, nullptr, nullptr);}
| TypeSpecifier IDENTIFIER '(' ')' compound_statement {$$ = new Function($1, *$2, nullptr, $5);}
;
// this is for function with definitions or other definitions also can
compound_statement
: '{' statement_list '}' {$$ = $2;}
| '{' '}'  {$$ = nullptr;}
;
statement_list
: statement  {new NodeList($1);}
| statement_list statement {$1 -> PushBack($2); $$ = $1;}
;
statement
: jump_statement';' {$$  = $1;}
| declaration ';' {$$ = $1;}
;

jump_statement
: RETURN  {$$ = new Return(NULL);}
| RETURN expr {$$ = new Return($2);}
;
// took it from the parser labs lol


declaration
:
;


expr
: BASE {$$ = $1;}
;


BASE
: INT_CONSTANT {$$ = new IntConstant($1);}
| IDENTIFIER 	{$$ = new Variable(*$1); delete $1;}
;

TypeSpecifier
: INT {$$ = new TypeSpecifier(_Types::_int);}
;
%%

Node *g_root;

Node *ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if(yyin == NULL){
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    exit(1);
  }
  g_root = nullptr;
  yyparse();
  return g_root;
}
