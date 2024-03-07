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
            int result = leftOperand_->getVal() * rightOperand_->getVal();
            stream << "li " << resultReg << ", " << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant"){

            if (leftOperand_->getType() == "constant") {
                std::string tmp = context.AllocReg("tmp");
                std::string dst = context.AllocReg(rightOperand_->getId());
                stream << "li " << tmp << ", " << leftOperand_->getVal() << std::endl;
                stream << "lw " << dst << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
                stream << "mul " << dst << ", " << dst << ", " << tmp << std::endl;
                context.DeallocReg(leftOperand_->getId());
                context.dst = rightOperand_->getId();
            }
            else {
                std::string dst = context.AllocReg(leftOperand_->getId());
                std::string tmp = context.AllocReg("tmp");
                stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
                stream << "li " << tmp << ", " << rightOperand_->getVal() << std::endl;
                stream << "mul " << dst << ", " << dst << ", " << tmp << std::endl;
                context.dst = leftOperand_->getId();
                context.DeallocReg(rightOperand_->getId());
            }
        }
        else {
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg(rightOperand_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "lw " << tmp << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "mul " << dst << ", " << dst << ", " << tmp << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg(rightOperand_->getId());
        }
    }
};

#endif
