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

// blocks
#include "ast/blocks/ast_ifelse.hpp"
#include "ast/blocks/ast_while.hpp"

// functions
#include "ast/functions/ast_function.hpp"

// jumps
#include "ast/jumps/ast_return.hpp"
#include "ast/functions/ast_call.hpp"

// keywords

// operators
#include "ast/operators/ast_assignment_operators.hpp"
#include "ast/operators/ast_arithmetic_operators.hpp"
#include "ast/operators/ast_bitwise_operators.hpp"
#include "ast/operators/ast_realational_operators.hpp"
#include "ast/operators/ast_logical_operators.hpp"


// pointers

// primitives
#include "ast/primitives/ast_primitives.hpp"

// types




extern Node *ParseAST(std::string file_name);

#endif
