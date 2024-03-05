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
        }
        else if (ret->getType() == "variable"){
            std:: string var = ret->getId();
            context.AllocReg(var);
            stream << "lw " << context.bindings[var] << "," << context.MemoryMapping[var] << "(sp)" << std::endl;
            stream << "mv a0," << context.bindings[var] << std::endl;
            context.DeallocReg(var);
        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif