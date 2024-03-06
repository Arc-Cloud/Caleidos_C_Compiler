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

// functions
#include "ast/functions/ast_function.hpp"

// jumps
#include "ast/jumps/ast_return.hpp"

// keywords

// operators
#include "ast/operators/ast_assign.hpp"
#include "ast/operators/ast_add.hpp"
#include "ast/operators/ast_sub.hpp"

// pointers

// primitives
#include "ast/primitives/ast_primitives.hpp"

// types




extern Node *ParseAST(std::string file_name);

#endif
