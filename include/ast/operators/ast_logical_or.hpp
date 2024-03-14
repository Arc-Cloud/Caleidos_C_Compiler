#ifndef AST_LOGICALOR_HPP
#define AST_LOGICALOR_HPP

#include "../ast_node.hpp"
class LogicalOr: public Node{
    private:
    Node* leftOperand_;
    Node* rightOperand_;
    public:

    std::string getType() const override {
        return "operator";
    };

    LogicalOr(Node* left, Node* right): leftOperand_(left), rightOperand_(right){};

    virtual ~LogicalOr(){
        delete leftOperand_;
        delete rightOperand_;
    }

    void Print(std::ostream &stream) const override{};
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (leftOperand_->getType() == "constant" && rightOperand_->getType() == "constant") {
            std::string resultReg = context.AllocReg("result");
            int result = leftOperand_->getVal() || rightOperand_->getVal();
            stream << "li " << resultReg << "," << result << std::endl;
            context.dst = "result";
        }
        else if (leftOperand_->getType() == "constant" || rightOperand_->getType() == "constant") {
            if (leftOperand_->getType() == "constant") {
                if(leftOperand_->getVal() != 0){
                    std::string resultReg = context.AllocReg("result");
                    int result = 1;
                    stream << "li " << resultReg << "," << result << std::endl;
                    context.dst = "result";
                }
                else{
                    std::string dst = context.AllocReg(rightOperand_->getId());
                    stream << "lw " << dst << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
                    stream << "snez " << dst << "," << dst << std::endl;
                    context.dst = rightOperand_->getId();
                }
            }
            else {
                if(rightOperand_->getVal() != 0){
                    std::string resultReg = context.AllocReg("result");
                    int result = 1;
                    stream << "li " << resultReg << "," << result << std::endl;
                    context.dst = "result";
                }
                else{
                std::string dst = context.AllocReg(leftOperand_->getId());
                stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
                stream << "snez " << dst << "," << dst << std::endl;
                context.dst = leftOperand_->getId();
                }
            }
        }
        else {
            std::string label1 = context.makeName("L");
            std::string label2 = context.makeName("L");
            std::string label3 = context.makeName("L");
            std::string dst = context.AllocReg(leftOperand_->getId());
            std::string tmp = context.AllocReg(rightOperand_->getId());
            stream << "lw " << dst << "," << context.MemoryMapping[leftOperand_->getId()] << "(sp)" << std::endl;
            stream << "bne " << dst << ",zero," << label1 << std::endl;
            stream << "lw " << tmp << "," << context.MemoryMapping[rightOperand_->getId()] << "(sp)" << std::endl;
            stream << "beq " << tmp << ",zero," << label2 << std::endl;
            stream << label1 << ":" << std::endl;
            stream << "li  " << dst << "," << 1 << std::endl;
            stream << "j " << label3 << std::endl;
            stream << label2 << ":" << std::endl;
            stream << "li  " << dst << "," <<  0 << std::endl;
            stream << label3 << ":" << std::endl;
            context.dst = leftOperand_->getId();
            context.DeallocReg(rightOperand_->getId());
        }

    }
};


#endif
