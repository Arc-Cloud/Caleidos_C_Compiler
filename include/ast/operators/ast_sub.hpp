#ifndef AST_SUB_HPP
#define AST_SUB_HPP

#include "../ast_node.hpp"

class Sub : public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;

    public:
    std::string getType() const override{
        return "operator";
    };

    Sub(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    virtual ~Sub() {
        delete leftOperand_;
        delete rightOperand_;
    }

    virtual void Print(std::ostream &stream) const override {};

    void EmitRISC(std::ostream &stream, Context &context) const override {
    std::string dst, src;

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() - rightOperand_->getVal();
            stream << "li " << resultReg << ", " << result << std::endl;
            context.DeallocReg("result");
        }
        else if (leftOperand_->getType() == "constant") {
            std::string leftReg = context.AllocReg(leftOperand_->getId());
            std::string rightReg = context.AllocReg(rightOperand_->getId());
            dst = leftReg;
            src = rightReg;
            stream << "li " << leftReg << ", " << leftOperand_->getVal() << std::endl;
            stream << "sub " << dst << ", " << dst << ", " << src << std::endl;
            context.DeallocReg(leftOperand_->getId());
            context.DeallocReg(rightOperand_->getId());
        }
        else {
            context.AllocReg(leftOperand_->getId());
            dst = context.bindings[leftOperand_->getId()];

            if (rightOperand_->getType() == "constant") {
                stream << "addi " << dst << ", " << dst << ", -" << rightOperand_->getVal() << std::endl;
            } else {
                context.AllocReg(rightOperand_->getId());
                src = context.bindings[rightOperand_->getId()];
                stream << "sub " << dst << ", " << dst << ", " << src << std::endl;
                context.DeallocReg(rightOperand_->getId());
            }

            context.DeallocReg(leftOperand_->getId());
        }
    }


};
#endif
