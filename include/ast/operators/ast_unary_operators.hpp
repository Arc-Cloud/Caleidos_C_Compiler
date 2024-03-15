#ifndef AST_UNARYOP_HPP
#define AST_UNARYOP_HPP

#include "../ast_node.hpp"


class UnaryMinusOp : public Node
{
private:
    Node *Operand;

public:
    UnaryMinusOp(Node *op) : Operand(op){};

    virtual ~UnaryMinusOp()
    {
        delete Operand;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (Operand->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = -Operand->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            Operand->EmitRISC(stream, context);
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "neg " << res << "," << context.bindings[context.dst] << std::endl;
            context.DeallocReg(context.dst);
            context.dst = op;
        }
    }

};


#endif
