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

        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() - rightOperand_->getVal();
            stream << "li " << resultReg << ", " << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant") {
            if (leftOperand_->getType() == "constant") {
            std::string tmp = context.AllocReg("tmp");
            std::string dst = context.AllocReg(rightOperand_->getId());
            stream << "li " << tmp << ", " << leftOperand_->getVal() << std::endl;
            stream << "sub " << dst << ", " << tmp << ", " << dst << std::endl;
            context.DeallocReg("tmp");
            context.dst = rightOperand_->getId();
            }
            else{
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg("tmp");
            stream << "li " << tmp << ", " << rightOperand_->getVal() << std::endl;
            stream << "sub " << dst << ", " << dst << ", " << tmp << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg("tmp");
            }
        }
        else {
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg(rightOperand_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "lw " << tmp << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "sub" << dst << ", " << dst << ", " << tmp << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg(rightOperand_->getId());
        }
    }


};
#endif
