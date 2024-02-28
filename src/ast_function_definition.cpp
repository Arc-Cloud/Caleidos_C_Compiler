#include "ast/functions/ast_function_definition.hpp"

void FunctionDefinition::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.
    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.
    stream << ".text" << std:: endl;
    stream << ".globl f" << std:: endl;
    context.WriteInstType("function");
    declarator_->EmitRISC(stream, context);
    stream << "addi	sp,sp,-16" << std:: endl;
	stream << "sw	s0,12(sp)" << std:: endl;
	stream << "addi	s0,sp,16" << std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->EmitRISC(stream, context);
    }

    stream << "lw	s0,12(sp)" << std:: endl;
	stream << "addi	sp,sp,16" << std::endl;
	stream << "ret" << std:: endl;

    // for now we implement the memory location manually first
    // we will deal with this properly in the future for memory management.
}

void FunctionDefinition::Print(std::ostream &stream) const
{
    declaration_specifiers_->Print(stream);
    stream << " ";

    declarator_->Print(stream);
    stream << "() {" << std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->Print(stream);
    }
    stream << "}" << std::endl;
}
