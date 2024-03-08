// NEED TO TEST

#ifndef AST_GREATERTHANEQUAL_HPP
#define AST_GREATERTHANEQUAL_HPP

#include "../ast_node.hpp"

class GreaterThanEqual : public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;

    public:

    GreaterThanEqual(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    virtual ~GreaterThanEqual() {
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override{
        return "operator";
    };



    virtual void Print(std::ostream &stream) const override {};

    void EmitRISC(std::ostream &stream, Context &context) const override {

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() > rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant") {
            if (leftOperand_->getType() == "constant") {
                std::string dst = context.AllocReg(rightOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
                stream << "slti " << dst << "," << dst << "," << leftOperand_->getVal() + 1 << std::endl;
                context.dst = rightOperand_->getId();
            }
            else {
                std::string dst = context.AllocReg(leftOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
                stream << "slti " << dst << "," << dst << "," << rightOperand_->getVal() << std::endl;
                stream << "xori " << dst << "," << dst << "," << 1 << std::endl;
                context.dst = leftOperand_->getId();
            }
        }
        else {
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg(rightOperand_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "lw " << tmp << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "slt " << dst << "," << dst << "," << tmp << std::endl;
            stream << "xori " << dst << "," << dst << "," << 1 << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg(rightOperand_->getId());
        }

    }

};
#endif
