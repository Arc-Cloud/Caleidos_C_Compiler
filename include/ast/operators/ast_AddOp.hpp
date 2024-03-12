#ifndef AST_ADDOP_HPP
#define AST_ADDOP_HPP

#include "../ast_node.hpp"

class AddOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    AddOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~AddOp()
    {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override
    {
        return "operator";
    };

    virtual void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant")
        {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() + rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);
            stream << "add " << res << "," << context.bindings[left] << "," << context.bindings[right] << std::endl;
            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }
};
#endif