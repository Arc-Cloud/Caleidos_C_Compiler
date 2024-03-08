#ifndef AST_RETURN_HPP
#define AST_RETURN_HPP

#include "../ast_node.hpp"

class Return : public Node
{
protected:
    Node *ret;

public:
    Return(Node *expr) : ret(expr){};

    virtual void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (ret->getType() == "constant")
        {
            stream << "li a0, " << ret->getVal() << std::endl;
            stream << "lw s0,"<< std::to_string(context.MemoryMapping["s0"])<< "(sp)" << std:: endl;
            stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
            stream << "addi sp,sp," << context.default_mem << std::endl;

            stream << "ret" << std::endl;

        }
        else if (ret->getType() == "variable"){
            std:: string var = ret->getId();
            context.AllocReg(var);
            stream << "lw " << context.bindings[var] << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
            stream << "mv a0," << context.bindings[var] << std::endl;
            context.DeallocReg(var);
            stream << "lw s0,"<< std::to_string(context.MemoryMapping["s0"])<< "(sp)" << std:: endl;
            stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
            stream << "addi sp,sp," << context.default_mem << std::endl;

            stream << "ret" << std::endl;

        }
        else if (ret -> getType() == "operator"){
            ret -> EmitRISC(stream, context);
            stream << "mv a0," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
            stream << "lw s0,"<< std::to_string(context.MemoryMapping["s0"])<< "(sp)" << std:: endl;
            stream << "lw ra," << std:: to_string(context.MemoryMapping["ra"]) << "(sp)" << std::endl;
            stream << "addi sp,sp," << context.default_mem << std::endl;

            stream << "ret" << std::endl;

        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif
