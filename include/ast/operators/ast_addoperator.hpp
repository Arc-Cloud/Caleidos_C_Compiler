#ifndef AST_ADDOPERATOR_HPP
#define AST_ADDOPERATOR_HPP

#include "../ast_node.hpp"

// class AddOperator: public Node{
//     private:
//     Node* LeftOp;
//     Node* RightOp;
//     public:
//     AddOperator(Node* left, Node* right): LeftOp(left), RightOp(right){};
//     virtual ~AddOperator(){
//         delete LeftOp;
//         delete RightOp;
//     }
//     std:: string getType() const override{
//         return "operator";
//     }

//     void Print(std::ostream &stream) const override{};

//     void EmitRISC(std::ostream &stream, Context &context) const override
//     {
//         int res;
//         if (LeftOp -> getType() == "constant" && RightOp -> getType() == "constant"){
//             res = LeftOp -> getVal() + RightOp -> getVal();
//             std:: string result = context.AllocReg("result");
//             stream << "li " << result << "," << res << std::endl;
//             context.dst = "result";
//         }
//         else if (LeftOp -> getType() == "constant" && RightOp -> getType() == "variable"){
//             std:: string reg1 = context.AllocReg(RightOp->getId());
//             stream << "lw " << reg1 << "," << context.MemoryMapping[RightOp->getId()] << "(sp)" << std::endl;
//             stream << "addi " << reg1 << "," << reg1 << "," << LeftOp -> getVal() << std::endl;
//             context.dst = RightOp->getId();
//         }
//     }

// };


class Add : public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;

    public:

    Add(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    virtual ~Add() {
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
            int result = leftOperand_->getVal() + rightOperand_->getVal();
            stream << "li " << resultReg << ", " << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant") {
            if (leftOperand_->getType() == "constant") {
                std::string dst = context.AllocReg(rightOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
                stream << "addi " << dst << ", " << dst << ", " << leftOperand_->getVal() << std::endl;
                context.dst = rightOperand_->getId();
            }
            else {
                std::string dst = context.AllocReg(leftOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
                stream << "addi " << dst << ", " << dst << ", " << rightOperand_->getVal() << std::endl;
                context.dst = leftOperand_->getId();
            }
        }
        else {
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg(rightOperand_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "lw " << tmp << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "add " << dst << ", " << dst << ", " << tmp << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg(rightOperand_->getId());
        }

    }

};
#endif


