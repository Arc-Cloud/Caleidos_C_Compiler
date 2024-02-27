#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast/ast_direct_declarator.hpp"
#include "ast/ast_function_definition.hpp"
#include "ast/ast_identifier.hpp"
#include "ast/ast_jump_statement.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_type_specifier.hpp"
#include "ast/ast_constant.hpp"
#include "ast/ast_context.hpp"

extern Node *ParseAST(std::string file_name);

#endif
