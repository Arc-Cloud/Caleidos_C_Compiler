#include "../include/ast/ast_declaration.hpp"
void declaration::Print(std::ostream &stream) const{}


void declaration::EmitRISC(std::ostream &stream, Context &context) const{
    std:: string DataType = type_spec->getType();
    if (DataType == "int"){
        context.MemAlloc(identifier_->getVar());
    }
};

