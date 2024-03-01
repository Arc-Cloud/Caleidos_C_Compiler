#include "ast/ast_type_specifier.hpp"

void TypeSpecifier::EmitRISC(std::ostream &stream, Context &context) const {}

void TypeSpecifier::Print(std::ostream &stream) const
{
}

std::string TypeSpecifier::getType() const
{
    switch (id)
    {
    case _int:
        return "int";
        break;
    }
}

