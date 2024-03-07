#ifndef AST_EQUAL_HPP
#define AST_EQUAL_HPP
#include "../ast_node.hpp"


class Equal: public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;
    public:
    Equal (Node* left, Node* right): leftOperand_(left), rightOperand_(right){};
    virtual ~Equal(){
        delete leftOperand_;
        delete rightOperand_;
    }

    std::string getType() const override {
        return "operator";
    }
    void Print(std::ostream &stream) const override {};

     void EmitRISC(std::ostream &stream, Context &context) const override {
         if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() == rightOperand_->getVal();
            stream << "li " << resultReg << ", " << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant"){

            if (leftOperand_->getType() == "constant") {
                std:: string dst = context.AllocReg(rightOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
                stream << "addi " << dst << "," << dst << "," << leftOperand_ -> getVal() << std::endl;
                stream << "seqz " << dst << "," << dst << std::endl;
                stream << "andi " << dst << "," << dst << ",0xff" << std::endl;
                context.dst = rightOperand_-> getId();

            }
            else {
                std:: string dst = context.AllocReg(leftOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
                stream << "addi " << dst << "," << dst << "," << rightOperand_ -> getVal() << std::endl;
                stream << "seqz " << dst << "," << dst << std::endl;
                stream << "andi " << dst << "," << dst << ",0xff" << std::endl;
                context.dst = leftOperand_-> getId();
            }
        }
        else {
            std:: string left = context.AllocReg(leftOperand_ -> getId());
            std:: string right = context.AllocReg(rightOperand_-> getId());
            stream << "lw " << left << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "lw " << right << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "sub " << right << "," << left << "," << right << std::endl;
            context.DeallocReg(leftOperand_->getId());
            stream << "seqz " << right << "," << right << std::endl;
            stream << "andi " << right << "," << right << ",0xff" << std::endl;
            context.dst = rightOperand_->getId();

        }
    }

};



#endif
