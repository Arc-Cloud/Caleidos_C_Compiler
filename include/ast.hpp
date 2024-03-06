#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast/ast_context.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_type_specifier.hpp"
#include "ast/primitives/ast_primitives.hpp"
#include "ast/jumps/ast_return.hpp"
#include "ast/functions/ast_function.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/operators/ast_assign.hpp"
#include "ast/operators/ast_addoperator.hpp"
extern Node *ParseAST(std::string file_name);

#endif
