#include "ast/ast_identifier.hpp"

void Identifier::EmitRISC(std::ostream &stream, Context &context) const
{
    if (context.ReadInstType() == "return"){
        stream << identifier_ << std::endl;
    }
    else if (context.ReadInstType() == "function"){
    stream << identifier_; //temporary needs to expand this for the future
    }
}

void Identifier::Print(std::ostream &stream) const
{
    stream << identifier_;
};

std:: string Identifier::getVar() const {
    return identifier_;
}
