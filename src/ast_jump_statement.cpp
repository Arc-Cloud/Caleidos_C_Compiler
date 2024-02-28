#include "ast/ast_jump_statement.hpp"

void ReturnStatement::EmitRISC(std::ostream &stream, Context &context) const
{
    Context _return;
    _return.WriteInstType("return");
    if (expression_ != nullptr)
    {
        expression_->EmitRISC(stream, _return);
    }
    stream << "jr ra" << std::endl;
}

void ReturnStatement::Print(std::ostream &stream) const
{
    stream << "return";
    if (expression_ != nullptr)
    {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";" << std::endl;
}
