#include "ast/ast_direct_declarator.hpp"

void DirectDeclarator::EmitRISC(std::ostream &stream, Context &context) const
{

    if (context.ReadInstType() == "return"){
        stream << "call ";
        identifier_->EmitRISC(stream,context);
    }
    else if (context.ReadInstType() == "function"){
    identifier_->EmitRISC(stream, context);
    stream << ":" << std::endl;}
    else if (context.ReadInstType() == "top"){
        stream << ".globl ";
        identifier_ ->EmitRISC(stream, context);
    }
}

void DirectDeclarator::Print(std::ostream &stream) const
{
    identifier_->Print(stream);
}
