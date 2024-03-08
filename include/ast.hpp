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

// functions
#include "ast/functions/ast_function.hpp"

// jumps
#include "ast/jumps/ast_return.hpp"

// keywords

// operators
#include "ast/operators/ast_assign.hpp"
#include "ast/operators/ast_add.hpp"
#include "ast/operators/ast_sub.hpp"
#include "ast/operators/ast_mul.hpp"
#include "ast/operators/ast_div.hpp"
#include "ast/operators/ast_bitwise_and.hpp"
#include "ast/operators/ast_bitwise_or.hpp"
#include "ast/operators/ast_bitwise_xor.hpp"
#include "ast/operators/ast_equal.hpp"
#include "ast/operators/ast_less_than.hpp"
#include "ast/operators/ast_less_than_equal.hpp"
#include "ast/operators/ast_greater_than.hpp"
#include "ast/operators/ast_greater_than_equal.hpp"
#include "ast/operators/ast_not_equal.hpp"
// pointers

// primitives
#include "ast/primitives/ast_primitives.hpp"

// types




extern Node *ParseAST(std::string file_name);

#endif
