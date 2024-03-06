#ifndef AST_MUL_HPP
#define AST_MUL_HPP

#include "../ast_node.hpp"

class Mul : public Node {
private:
    Node* leftOperand_;
    Node* rightOperand_;

public:
    std::string getType() const override {
        return "operator";
    };

    Mul(Node* left, Node* right): leftOperand_(left), rightOperand_(right) {};

    virtual ~Mul() {
        delete leftOperand_;
        delete rightOperand_;
    }

    void Print(std::ostream &stream) const override {};

    void EmitRISC(std::ostream &stream, Context &context) const override {
        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int resultVal = leftOperand_->getVal() * rightOperand_->getVal();
            stream << "li " << resultReg << ", " << resultVal << std::endl;
            context.DeallocReg("result");
        }
        else {
            std::string leftReg, rightReg, dst;

            if (leftOperand_->getType() == "constant") {
                // Left operand is constant, load it into a register.
                int constVal = leftOperand_->getVal();
                leftReg = context.AllocReg("tmp");
                stream << "li " << leftReg << ", " << constVal << std::endl;
                rightReg = context.AllocReg(rightOperand_->getId());
            } else if (rightOperand_->getType() == "constant") {
                // Right operand is constant, load it into a register.
                int constVal = rightOperand_->getVal();
                rightReg = context.AllocReg("tmp");
                stream << "li " << rightReg << ", " << constVal << std::endl;
                leftReg = context.AllocReg(leftOperand_->getId());
            } else {
                // Neither operand is a constant.
                leftReg = context.AllocReg(leftOperand_->getId());
                rightReg = context.AllocReg(rightOperand_->getId());
            }

            dst = leftReg; // Assuming result is stored in the left operand's register (or a new register if needed).
            stream << "mul " << dst << ", " << leftReg << ", " << rightReg << std::endl;

            // Deallocation of temporary registers if necessary.
            context.DeallocReg(leftOperand_->getType() == "constant" ? "tmp" : leftOperand_->getId());
            context.DeallocReg(rightOperand_->getType() == "constant" ? "tmp" : rightOperand_->getId());
        }
    }
};

#endif
