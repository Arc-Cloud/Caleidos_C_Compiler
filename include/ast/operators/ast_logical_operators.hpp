#ifndef AST_LOGICALOP_HPP
#define AST_LOGICALOP_HPP

#include "../ast_node.hpp"


class LogicalAndOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    LogicalAndOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~LogicalAndOp()
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
            int result = leftOperand_->getVal() && rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string label1 = context.makeName("L");
            std::string label2 = context.makeName("L");

            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);

            stream << "beq " << context.bindings[left] << ",zero," << label1 << std::endl;
            stream << "beq " << context.bindings[right] << ",zero," << label1 << std::endl;
            stream << "li  " << res << "," << 1 << std::endl;
            stream << "j " << label2 << std::endl;
            stream << label1 << ":" << std::endl;
            stream << "li " << res << "," <<  0 << std::endl;
            stream << label2 << ":" << std::endl;

            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};

class LogicalOrOp : public Node
{
private:
    Node *leftOperand_;
    Node *rightOperand_;

public:
    LogicalOrOp(Node *left, Node *right) : leftOperand_(left), rightOperand_(right){};

    virtual ~LogicalOrOp()
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
            int result = leftOperand_->getVal() || rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else
        {
            std::string label1 = context.makeName("L");
            std::string label2 = context.makeName("L");
            std::string label3 = context.makeName("L");

            leftOperand_->EmitRISC(stream, context);
            std:: string left = context.dst;
            rightOperand_->EmitRISC(stream, context);
            std:: string right= context.dst;
            std:: string op = context.makeName("O");
            std:: string res = context.AllocReg(op);

            stream << "bne " << context.bindings[left] << ",zero," << label1 << std::endl;
            stream << "beq " << context.bindings[right] << ",zero," << label2 << std::endl;
            stream << label1 << ":" << std::endl;
            stream << "li  " << res << "," << 1 << std::endl;
            stream << "j " << label3 << std::endl;
            stream << label2 << ":" << std::endl;
            stream << "li  " << res << "," <<  0 << std::endl;
            stream << label3 << ":" << std::endl;


            context.DeallocReg(left);
            context.DeallocReg(right);
            context.dst = op;
        }
    }

};


#endif
