#ifndef AST_ADD_HPP
#define AST_ADD_HPP

#include "../ast_node.hpp"

class Add : public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;

    public:

    Add(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    std::string getType() const override{
        return "operator";
    };

    virtual ~Add() {
        delete leftOperand_;
        delete rightOperand_;
    }

    virtual void Print(std::ostream &stream) const override {};

    void EmitRISC(std::ostream &stream, Context &context) const override {
        std::string dst, src;

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int resultVal = leftOperand_->getVal() + rightOperand_->getVal();
            stream << "li " << resultReg << ", " << resultVal << std::endl;
            context.DeallocReg("result");
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant") {
            if (leftOperand_->getType() == "constant") {
                std::string rightReg = context.AllocReg(rightOperand_->getId());
                dst = rightReg;
                int constVal = leftOperand_->getVal();
                stream << "addi " << dst << ", " << dst << ", " << constVal << std::endl;
                context.DeallocReg(rightOperand_->getId());
            }
            else {
                std::string leftReg = context.AllocReg(leftOperand_->getId());
                dst = leftReg;
                int constVal = rightOperand_->getVal();
                stream << "addi " << dst << ", " << dst << ", " << constVal << std::endl;
                context.DeallocReg(leftOperand_->getId());
            }
        }
        else {
            std::string leftReg = context.AllocReg(leftOperand_->getId());
            std::string rightReg = context.AllocReg(rightOperand_->getId());
            dst = leftReg;
            src = rightReg;
            stream << "add " << dst << ", " << dst << ", " << src << std::endl;
            context.DeallocReg(leftOperand_->getId());
            context.DeallocReg(rightOperand_->getId());
        }

    }

};
#endif
