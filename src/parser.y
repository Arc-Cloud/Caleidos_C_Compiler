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

%type <node> external_declaration function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list
%type <node> init_declarator type_specifier struct_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <node> struct_declarator enum_specifier enumerator declarator direct_declarator pointer  parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator initializer statement labeled_statement
%type <node> compound_statement expression_statement selection_statement iteration_statement jump_statement

%type <nodes> statement_list translation_unit declaration_list initializer_list parameter_list argument_expression_list enumerator_list

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
    | FLOAT_CONSTANT {$$ = new FloatConstant($1);}
	| STRING_LITERAL
	| '(' expression ')' {$$ = $2;}
	;

postfix_expression
	: primary_expression {$$ = $1;}
	| postfix_expression '[' expression ']' {$$ = new ArrayIndex($1,$3);}
	| postfix_expression '(' ')' {$$ = new Call($1, NULL);}
	| postfix_expression '(' argument_expression_list ')' {$$ = new Call($1, $3);}
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP {$$ = new UnaryIncrOp($1);}
	| postfix_expression DEC_OP {$$ = new UnaryDecrOp($1);}
	;

argument_expression_list
	: assignment_expression {$$ =  new Argument($1);}
	| argument_expression_list ',' assignment_expression {$1 -> PushBack($3); $$ = $1;}
	;

unary_expression
	: postfix_expression {$$ = $1;}
	| INC_OP unary_expression //{$$ = new ($2);}
	| DEC_OP unary_expression //{$$ = new ($2);}
	| '&' unary_expression
	| '*' unary_expression
  	| '+' unary_expression {$$ = $2;}
	| '-' unary_expression {$$ = new UnaryMinusOp($2);}
	| '~' unary_expression {$$ = new UnaryBitwiseNotOp($2);}
	| '!' unary_expression {$$ = new UnaryLogicNotOp($2);}
	| SIZEOF unary_expression {$$ = new UnarySizeOfOp($2);}
	| SIZEOF '(' type_name ')' {$$ = new UnarySizeOfOp($3);}
	;

cast_expression
	: unary_expression {$$ = $1;}
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression {$$ = $1;}
	| multiplicative_expression '*' cast_expression {$$ = new MulOp($1, $3);}
	| multiplicative_expression '/' cast_expression {$$ = new DivOp($1, $3);}
	| multiplicative_expression '%' cast_expression {$$ = new ModOp($1, $3);}
	;

additive_expression
	: multiplicative_expression {$$ = $1;}
	| additive_expression '+' multiplicative_expression {$$ = new AddOp($1, $3);}
	| additive_expression '-' multiplicative_expression {$$ = new SubOp($1,$3);}
	;

shift_expression
	: additive_expression {$$ = $1;}
	| shift_expression LEFT_OP additive_expression {$$ = new ShiftLeftOp($1,$3);}
	| shift_expression RIGHT_OP additive_expression {$$ = new ShiftRightOp($1,$3);}
	;

relational_expression
	: shift_expression {$$ = $1;}
	| relational_expression '<' shift_expression {$$ = new LessThanOp($1,$3);}
	| relational_expression '>' shift_expression {$$ = new GreaterThanOp($1,$3);}
	| relational_expression LE_OP shift_expression {$$ = new LessThanEqualOp($1,$3);}
	| relational_expression GE_OP shift_expression {$$ = new GreaterThanEqualOp($1,$3);}
	;

equality_expression
	: relational_expression {$$ = $1;}
	| equality_expression EQ_OP relational_expression {$$ = new EqualOp($1,$3);}
	| equality_expression NE_OP relational_expression {$$ = new NotEqualOp($1,$3);}
	;

and_expression
	: equality_expression {$$ = $1;}
	| and_expression '&' equality_expression {$$ = new BitwiseAndOp($1,$3);}
	;

exclusive_or_expression
	: and_expression {$$ = $1;}
	| exclusive_or_expression '^' and_expression {$$ = new BitwiseXorOp($1,$3);}
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression {$$ = new BitwiseOrOp($1,$3);}
	;

logical_and_expression
	: inclusive_or_expression {$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression {$$ = new LogicalAndOp($1,$3);}
	;

logical_or_expression
	: logical_and_expression {$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression {$$ = new LogicalOrOp($1,$3);}
	;

conditional_expression
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression {$$ = new TernaryOp($1,$3,$5);}
	;
assignment_expression
	: conditional_expression {$$ = $1;}
    | unary_expression '=' assignment_expression {$$ = new Assign($1, $3);}
	| unary_expression MUL_ASSIGN assignment_expression {$$ = new Assign($1, new MulOp($1, $3));}
    | unary_expression DIV_ASSIGN assignment_expression {$$ = new Assign ($1, new DivOp($1, $3));}
    | unary_expression MOD_ASSIGN assignment_expression {$$ = new Assign ($1, new ModOp($1, $3));}
    | unary_expression ADD_ASSIGN assignment_expression {$$ = new Assign($1, new AddOp($1,$3));}
    | unary_expression SUB_ASSIGN assignment_expression {$$ = new Assign($1, new SubOp($1, $3));}
    | unary_expression LEFT_ASSIGN assignment_expression {$$ = new Assign($1, new ShiftLeftOp($1, $3));}
    | unary_expression RIGHT_ASSIGN assignment_expression {$$ = new Assign($1, new ShiftRightOp($1, $3));}
    | unary_expression AND_ASSIGN assignment_expression {$$ = new Assign($1, new BitwiseAndOp($1, $3));}
    | unary_expression XOR_ASSIGN assignment_expression {$$ = new Assign($1, new BitwiseXorOp($1, $3));}
    | unary_expression OR_ASSIGN assignment_expression {$$ = new Assign($1, new BitwiseOrOp($1, $3));}
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
	| FLOAT {$$ =  new TypeSpecifier(_Types:: _float);}
	| DOUBLE
	| SIGNED
	| UNSIGNED
    | struct_specifier
	| enum_specifier {$$ = $1;}
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
	: ENUM '{' enumerator_list '}' {$$ = new EnumDeclerator(NULL,$3);}
	| ENUM IDENTIFIER '{' enumerator_list '}' {$$ = new EnumDeclerator(new Variable(*$2), $4); delete $2;}
	| ENUM IDENTIFIER {$$ = new EnumDeclerator(new Variable(*$2), NULL); delete $2;}
	;

enumerator_list
	: enumerator {$$ = new NodeList($1);}
	| enumerator_list ',' enumerator {$1 -> PushBack($3); $$ = $1;}
	;

enumerator
	: IDENTIFIER {$$ = new Enumerator(new Variable(*$1) , NULL); delete $1;}
	| IDENTIFIER '=' constant_expression {$$ = new Enumerator(new Variable(*$1) , $3); delete $1;}
	;

declarator
	: pointer direct_declarator
	| direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER {$$ = new Variable(*$1);delete $1;}
	| '(' declarator ')' {$$ = $2;}
	| direct_declarator '[' constant_expression ']' {$$ = new DeclareArray($1, $3);}
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
	: '(' abstract_declarator ')' {$$ = $2;}
	| '[' ']' // wtf is this
	| '[' constant_expression ']' // no need
	| direct_abstract_declarator '[' ']' //no need
	| direct_abstract_declarator '[' constant_expression ']' // no need
	| '(' ')' //no need
	| '(' parameter_list ')' // no need
	| direct_abstract_declarator '(' ')' // no need
	| direct_abstract_declarator '(' parameter_list ')' // no need
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
	| compound_statement {$$ = $1;}
	| expression_statement {$$ = $1;}
	| selection_statement
	| iteration_statement
	| jump_statement { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement {$$ = new Case($2,$4);}
	| DEFAULT ':' statement {$$ = new Default($3);}
	;

compound_statement
	: '{' '}' {
		$$ = new CompoundStat(NULL, NULL);
	}
	| '{' statement_list '}' {
		$$ = $2;
	}
	| '{' declaration_list '}' {
		$$ = new CompoundStat($2, NULL);
	}
	| '{' declaration_list statement_list '}'  {
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
	: IF '(' expression ')' statement {$$ = new IfNoElse($3, $5);}
	| IF '(' expression ')' statement ELSE statement {$$ = new IfElse($3, $5,$7);}
	| SWITCH '(' expression ')' statement {$$ = new Switch($3,$5);}
	;

iteration_statement
	: WHILE '(' expression ')' statement {$$ = new While($3,$5);}
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement {$$ = new For($3,$4, NULL, $6);}
	| FOR '(' expression_statement expression_statement expression ')' statement {$$ = new For($3,$4, $5, $7);}
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';' {$$ = new Continue();}
	| BREAK ';' {$$ = new Break();}
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
