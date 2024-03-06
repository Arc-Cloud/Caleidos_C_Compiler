#ifndef AST_ADD_HPP
#define AST_ADD_HPP

#include "../ast_node.hpp"

class Add : public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;

    public:
    std::string getType() const override{
        return "operator"
    };

    Add(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    virtual ~Add() {
        delete leftOperand_;
        delete rightOperand_;
    }

    virtual void Print(std::ostream &stream) const override {};

    void EmitRISC(std::ostream &stream, Context &context) const override {
        std::string dst, src;

        context.AllocReg(leftOperand_->getId());
        dst = context.bindings[leftOperand_->getId()];

        if (rightOperand_->getType() == "constant") {
            stream << "addi " << dst << ", " << dst << ", " << rightOperand_->getVal() << std::endl;
        }
        else {
            context.AllocReg(rightOperand_->getId());
            src = context.bindings[rightOperand_->getId()];
            stream << "add " << dst << ", " << dst << ", " << src << std::endl;
            context.DeallocReg(rightOperand_->getId());
        }


        context.DeallocReg(leftOperand_->getId());
    };
};
#endif
