#include "ast/ast_identifier.hpp"

void Identifier::EmitRISC(std::ostream &stream, Context &context) const
{
    if (context.ReadInstType() == "return"){
        stream << identifier_ << std::endl;
    }
    else if (context.ReadInstType() == "function"){
    stream << identifier_; //temporary needs to expand this for the future
    }
    else if (context.ReadInstType() == "top"){
        stream << identifier_ << std:: endl;
    }
}

void Identifier::Print(std::ostream &stream) const
{
    stream << identifier_;
};
