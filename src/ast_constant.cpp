#include "ast/ast_constant.hpp"

void IntConstant::EmitRISC(std::ostream &stream, Context &context) const
{   
    std::string InstType = context.ReadInstType();
    if (InstType == "return"){
    stream << "li a0, " << value_ << std::endl;
    }
}

void IntConstant::Print(std::ostream &stream) const
{
    stream << value_;
}
