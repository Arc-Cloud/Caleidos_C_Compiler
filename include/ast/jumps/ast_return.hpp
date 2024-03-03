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
        else if (ret -> getType() == "variable"){
            stream << "lw a0," << context.MemoryMapping[ret->getId()][0] <<"(s0)"<< std:: endl;
        }
    }
    virtual void Print(std::ostream &stream) const override{};
};

#endif
