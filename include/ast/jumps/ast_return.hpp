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
        else {
            stream << "lw " << context.bindings["x"] << "," << context.MemoryMapping["x"] << "(sp)" << std::endl;
            stream << "mv a0," << context.bindings["x"] << std::endl;
        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif
