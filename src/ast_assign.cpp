#include "../include/ast/operators/ast_assign.hpp"


void SimpleAssignment::EmitRISC(std::ostream &stream, Context &context) const{
    stream << "li a5,";
    context.WriteInstType("simpleassign");
    _val ->EmitRISC(stream, context);
    stream << "sw a5," << context.GetMem(_expr->getVar()) << std:: endl;
}

void SimpleAssignment:: Print(std::ostream &stream) const{};
