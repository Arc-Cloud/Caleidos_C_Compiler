#ifndef AST_ADDOPERATOR_HPP
#define AST_ADDOPERATOR_HPP

#include "../ast_node.hpp"

class AddOperator: public Node{
    private:
    Node* LeftOp;
    Node* RightOp;
    public:
    AddOperator(Node* left, Node* right): LeftOp(left), RightOp(right){};
    virtual ~AddOperator(){
        delete LeftOp;
        delete RightOp;
    }
    std:: string getType() const override{
        return "operator";
    }

    void Print(std::ostream &stream) const override{};

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        int res;
        if (LeftOp -> getType() == "constant" && RightOp -> getType() == "constant"){
            res = LeftOp -> getVal() + RightOp -> getVal();
            std:: string result = context.AllocReg("result");
            stream << "li " << result << "," << res << std::endl;
            context.dst = "result";
        }
        else if (LeftOp -> getType() == "constant" && RightOp -> getType() == "variable"){
            std:: string reg1 = context.AllocReg(RightOp->getId());
            stream << "lw " << reg1 << "," << context.MemoryMapping[RightOp->getId()] << "(sp)" << std::endl;
            stream << "addi " << reg1 << "," << reg1 << "," << LeftOp -> getVal() << std::endl;
            context.dst = RightOp->getId();
        }
    }

};


#endif
