// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

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

%type <node> external_declaration function_definition primary_expression postfix_expression argument_expression_list
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list
%type <node> init_declarator type_specifier struct_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <node> struct_declarator enum_specifier enumerator_list enumerator declarator direct_declarator pointer  parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator initializer statement labeled_statement
%type <node> compound_statement expression_statement selection_statement iteration_statement jump_statement

%type <nodes> statement_list translation_unit declaration_list initializer_list parameter_list

%type <string> unary_operator assignment_operator storage_class_specifier

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER


%start ROOT
%%

ROOT
  : translation_unit { g_root = $1; }

translation_unit
	: external_declaration { $$ = new NodeList($1);}
	| translation_unit external_declaration{$1 -> PushBack($2); $$ = $1;}
	;

external_declaration
	: function_definition { $$ = $1; }
	| declaration
	;

function_definition
	: declaration_specifiers declarator compound_statement {$$ = new Function($1, $2, $3);}
	;

primary_expression
	: IDENTIFIER {$$ = new Variable(*$1); delete $1;}
	| INT_CONSTANT {$$ = new IntConstant($1);}
    | FLOAT_CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression {$$ = $1;}
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression {$$ = $1;}
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression {$$ = $1;}
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| '&' unary_expression
	| '*' unary_expression
  	| '+' unary_expression
	| '-' unary_expression
	| '~' unary_expression
	| '!' unary_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

cast_expression
	: unary_expression {$$ = $1;}
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression {$$ = $1;}
	| multiplicative_expression '*' cast_expression {$$ = new Mul($1, $3);}
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression {$$ = $1;}
	| additive_expression '+' multiplicative_expression {$$ = new Add($1, $3);}
	| additive_expression '-' multiplicative_expression {$$ = new Sub($1,$3);}
	;

shift_expression
	: additive_expression {$$ = $1;}
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression {$$ = $1;}
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression {$$ = $1;}
	| equality_expression EQ_OP relational_expression {$$ = new Equal($1, $3);}
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression {$$ = $1;}
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression {$$ = $1;}
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression {$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression {$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression
	;
assignment_expression
	: conditional_expression {$$ = $1;}
    | unary_expression '=' assignment_expression {$$ = new Assign($1, $3);}
	| unary_expression MUL_ASSIGN assignment_expression
    | unary_expression DIV_ASSIGN assignment_expression
    | unary_expression MOD_ASSIGN assignment_expression
    | unary_expression ADD_ASSIGN assignment_expression
    | unary_expression SUB_ASSIGN assignment_expression
    | unary_expression LEFT_ASSIGN assignment_expression
    | unary_expression RIGHT_ASSIGN assignment_expression
    | unary_expression AND_ASSIGN assignment_expression
    | unary_expression XOR_ASSIGN assignment_expression
    | unary_expression OR_ASSIGN assignment_expression
	;

expression
	: assignment_expression {$$ = $1;}
	| expression ',' assignment_expression // not needed
	;

constant_expression
	: conditional_expression {$$ = $1;}
	;
declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator ';' {$$ = new Declaration($1, $2);}
	;
declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers //not needed
	;

init_declarator
	: declarator {$$ = $1;}
	| declarator '=' initializer {$$ = new InitDeclarator($1, $3);}
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT {$$ = new TypeSpecifier(_Types::_int);}
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
    | struct_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_specifier
	: STRUCT IDENTIFIER '{' struct_declaration_list '}'
	| STRUCT '{' struct_declaration_list '}'
	| STRUCT IDENTIFIER
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator {$$ = $1;}
	| ':' constant_expression // not needed
	| declarator ':' constant_expression //not needed
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

declarator
	: pointer direct_declarator
	| direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER {$$ = new Variable(*$1);delete $1;}
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_list ')' {$$ = new FunctionDeclarator($1, $3);}
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')' {$$ = new FunctionDeclarator($1, NULL);}
	;

pointer
	: '*'
	| '*' pointer
	;

parameter_list
	: parameter_declaration {$$ = new NodeList($1);}
	| parameter_list ',' parameter_declaration {$1 -> PushBack($3); $$ = $1;}
	;

parameter_declaration
	: declaration_specifiers declarator {$$ = new Declaration($1,$2);}
	| declaration_specifiers abstract_declarator
	| declaration_specifiers // not needed i guess
	;
identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;
type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_list ')'
	;

initializer
	: assignment_expression {$$ = $1;}
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer {$$ = new NodeList($1);}
	| initializer_list ',' initializer {$1 -> PushBack($3); $$ = $1;}
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement {$$ = $1;}
	| selection_statement
	| iteration_statement
	| jump_statement { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}' {
		// TODO: correct this
		$$ = new CompoundStat(NULL, NULL);
	}
	| '{' statement_list '}' {
		$$ = $2;
	}
	| '{' declaration_list '}' {
		// TODO: correct this
		$$ = new CompoundStat($2, NULL);
	}
	| '{' declaration_list statement_list '}'  {
		// TODO: correct this
		$$ = new CompoundStat($2, $3);
	}
	;

declaration_list
	: declaration {$$ = new NodeList($1);}
	| declaration_list declaration {$1 -> PushBack($2); $$ = $1;}
	;


statement_list
	: statement { $$ = new NodeList($1); }
	| statement_list statement { $1->PushBack($2); $$=$1; }
	;

expression_statement
	: ';'
	| expression ';' { $$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';' {$$ = new Return(nullptr);}
	| RETURN expression ';' {$$ = new Return($2);}
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
