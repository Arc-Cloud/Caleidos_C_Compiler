#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast/ast_context.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_type_specifier.hpp"

// arrays
#include "ast/arrays/ast_array.hpp"
#include "ast/arrays/ast_arrayindex.hpp"

// blocks
#include "ast/blocks/ast_ifelse.hpp"
#include "ast/blocks/ast_while.hpp"
#include "ast/blocks/ast_for.hpp"
#include "ast/blocks/ast_switch.hpp"

// functions
#include "ast/functions/ast_function.hpp"
#include "ast/functions/ast_argument.hpp"

// keywords
#include "ast/keywords/ast_return.hpp"
#include "ast/keywords/ast_call.hpp"
#include "ast/keywords/ast_enum.hpp"



// operators
#include "ast/operators/ast_assignment_operators.hpp"
#include "ast/operators/ast_arithmetic_operators.hpp"
#include "ast/operators/ast_bitwise_operators.hpp"
#include "ast/operators/ast_relational_operators.hpp"
#include "ast/operators/ast_logical_operators.hpp"
#include "ast/operators/ast_unary_operators.hpp"


// pointers

// primitives
#include "ast/primitives/ast_primitives.hpp"

// types




extern Node *ParseAST(std::string file_name);

#endif
